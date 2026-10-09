#include "View.hpp"

#include "Actions.hpp"
#include "Widgets.hpp"
#include "imgui.h"

#include <algorithm>
#include <cstdio>
#include <string>

void renderWindow(RunState& state) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Laboratory 1", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Laboratory 1 · Numerical linear algebra · Fall 2026");
    if (ImGui::BeginTable("panells", 2, ImGuiTableFlags_Resizable)) {
        ImGui::TableNextColumn();
        ImGui::SeparatorText("Data");
        std::string selectedDataset = "No file";
        if (!state.files.empty()) {
            state.selection = std::clamp(state.selection, 0, static_cast<int>(state.files.size()) - 1);
            selectedDataset = state.files[state.selection].filename().string();
        }
        if (ImGui::BeginCombo("Included dataset", selectedDataset.c_str())) {
            for (std::size_t i = 0; i < state.files.size(); ++i) {
                if (ImGui::Selectable(state.files[i].filename().string().c_str(),
                                      state.selection == static_cast<int>(i))) {
                    state.selection = static_cast<int>(i);
                    std::snprintf(state.path, sizeof(state.path), "%s", state.files[i].string().c_str());
                }
            }
            ImGui::EndCombo();
        }
        ImGui::InputText(".lab file", state.path, sizeof(state.path));
        if (ImGui::Button("Load file")) loadDatasetFromPath(state);

        const char* tasks[] = {"Identity", "Product Ax", "Product AB", "System Ax=b"};
        ImGui::Combo("New manual case", &state.task, tasks, 4);
        ImGui::InputInt("Rows m / system n", &state.rows);
        ImGui::BeginDisabled(state.task == 0 || state.task == 3);
        ImGui::InputInt("Inner dimension k", &state.inner);
        ImGui::EndDisabled();
        ImGui::BeginDisabled(state.task != 2);
        ImGui::InputInt("Columns p of B", &state.columns);
        ImGui::EndDisabled();
        state.rows = std::clamp(state.rows, 1, 8);
        state.inner = std::clamp(state.inner, 1, 8);
        state.columns = std::clamp(state.columns, 1, 8);
        if (ImGui::Button("Create editable manual case")) createManualCase(state);
        ImGui::TextWrapped("Active case: %s", state.data.name.c_str());
        ImGui::Text("Active operation: %s", tasks[static_cast<int>(state.data.task)]);
        if (state.data.task == Task::Solve && ImGui::Checkbox("Partial pivoting", &state.pivoting))
            clearReport(state);

        if (state.data.task == Task::Solve) {
            double tolerance = state.data.pivotTolerance;
            if (ImGui::InputDouble("Pivot tolerance", &tolerance, 0, 0, "%.2e"))
                updatePivotTolerance(state, tolerance);
        }

        const bool editable = !state.data.hasExpected && state.data.A.rows <= 8 && state.data.A.cols <= 8;
        bool changed = ui::matrixTable("A", state.data.A, editable);
        if (state.data.task == Task::MatrixMatrix) changed |= ui::matrixTable("B", state.data.B, editable);
        if (state.data.task == Task::MatrixVector || state.data.task == Task::Solve)
            changed |= ui::vectorTable(state.data.task == Task::Solve ? "b" : "x", state.data.input, editable);
        if (changed) clearReport(state);

        ImGui::TableNextColumn();
        ImGui::SeparatorText("Validation");
        if (ImGui::Button("Run operation")) runCurrentTask(state);
        if (!state.error.empty()) ImGui::TextWrapped("Error: %s", state.error.c_str());

        if (state.hasReport) {
            ui::feedback("Result", state.report.correct);
            if (!state.report.detail.empty()) ImGui::TextWrapped("%s", state.report.detail.c_str());
            ImGui::Text("Time: %.3f ms", state.report.milliseconds);
            if (state.report.countsApplicable) {
                ImGui::TextColored(state.report.countsCorrect ? ImVec4(0.4f, 0.9f, 0.5f, 1.0f)
                                                              : ImVec4(1.0f, 0.75f, 0.3f, 1.0f),
                                   "Operation count: %s",
                                   state.report.countsCorrect ? "WITHIN MARGIN" : "OUTSIDE MARGIN");
                ImGui::TextUnformatted("Reference margin: +/-10% relative to theory.");
                ui::operationCounts("Product", state.report.product);
                ui::operationCounts("Elimination", state.report.elimination);
                ui::operationCounts("Substitution", state.report.substitution);
            }
            if (state.report.timeApplicable) {
                ImGui::TextColored(state.report.timeExpected ? ImVec4(0.4f, 0.9f, 0.5f, 1.0f)
                                                             : ImVec4(1.0f, 0.75f, 0.3f, 1.0f),
                                   "Expected time: %s",
                                   state.report.timeExpected ? "WITHIN RANGE" : "OUTSIDE RANGE");
                ImGui::Text("Reference range: [%.3f, %.3f] ms", state.report.minMilliseconds,
                            state.report.maxMilliseconds);
            }
            if (state.data.task == Task::Solve && state.report.completed) {
                ImGui::Text("Computed residual: %.6e", state.report.studentResidual);
                ImGui::Text("Reference residual: %.6e", state.report.residual);
                ui::feedback("Residual function", state.report.residualCorrect && state.report.calibrationCorrect);
                if (!state.report.residualCorrect || !state.report.calibrationCorrect)
                    ImGui::TextWrapped("Check relativeResidual: residual is not computed correctly.");
            }
            if (state.data.hasExpected && state.report.completed)
                ImGui::Text("Solution error: %.6e", state.report.error);
            if (state.report.completed) ImGui::Text("Maximum allowed error: %.1e", state.data.accuracy);
            if (state.report.completed) {
                if (state.report.resultCols > 1) {
                    Matrix result(state.report.resultRows, state.report.resultCols);
                    result.values = state.report.result;
                    ui::matrixTable("Result", result, false);
                } else {
                    ui::vectorTable("Result", state.report.result, false);
                }
            }
        }

        ImGui::Separator();
        ImGui::EndTable();
    }
    ImGui::End();
}
