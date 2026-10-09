#include "Widgets.hpp"
#include "imgui.h"
#include <algorithm>

namespace ui {

bool matrixTable(const char* label, Matrix& matrix, bool editable) {
    bool changed = false;
    ImGui::PushID(label);
    ImGui::SeparatorText(label);
    const int rows = static_cast<int>(std::min<std::size_t>(matrix.rows, 6));
    const int cols = static_cast<int>(std::min<std::size_t>(matrix.cols, 6));
    ImGui::Text("%zu x %zu", matrix.rows, matrix.cols);
    if (cols && ImGui::BeginTable(label, cols, ImGuiTableFlags_Borders)) {
        for (int i = 0; i < rows; ++i) {
            ImGui::TableNextRow();
            for (int j = 0; j < cols; ++j) {
                ImGui::TableSetColumnIndex(j);
                double& value = matrix.values[i * matrix.cols + j];
                if (editable) {
                    ImGui::PushID(i * cols + j);
                    ImGui::SetNextItemWidth(-1);
                    changed |= ImGui::InputDouble("##valor", &value, 0, 0, "%.6g");
                    ImGui::PopID();
                } else {
                    ImGui::Text("%.6g", value);
                }
            }
        }
        ImGui::EndTable();
    }
    if (matrix.rows > 6 || matrix.cols > 6)
        ImGui::TextDisabled("Partial view; full data is used when executing.");
    ImGui::PopID();
    return changed;
}

bool vectorTable(const char* label, Vector& vector, bool editable) {
    bool changed = false;
    ImGui::SeparatorText(label);
    for (std::size_t i = 0; i < std::min<std::size_t>(vector.size(), 8); ++i) {
        ImGui::PushID(static_cast<int>(i));
        if (editable) {
            ImGui::SetNextItemWidth(140);
            changed |= ImGui::InputDouble(label, &vector[i], 0, 0, "%.6g");
        } else {
            ImGui::Text("[%zu] %.10g", i, vector[i]);
        }
        ImGui::PopID();
    }
    if (vector.size() > 8) ImGui::TextDisabled("... (%zu components)", vector.size());
    return changed;
}

void feedback(const char* label, bool ok) {
    ImGui::TextColored(ok ? ImVec4(0.4f, 0.9f, 0.5f, 1.0f) : ImVec4(1.0f, 0.45f, 0.35f, 1.0f),
                       "%s: %s", label, ok ? "CORRECT" : "REVIEW");
}

void operationCounts(const char* label, const Operations& op) {
    ImGui::Text("%s: + %llu | - %llu | * %llu | / %llu", label,
                static_cast<unsigned long long>(op.add),
                static_cast<unsigned long long>(op.subtract),
                static_cast<unsigned long long>(op.multiply),
                static_cast<unsigned long long>(op.divide));
}

} // namespace ui
