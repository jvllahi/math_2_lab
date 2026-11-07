#include "Validation.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

static bool vectorEdit(const char* label, Vec3& v) {
    ImGui::SeparatorText(label);
    ImGui::PushID(label);
    bool changed=ImGui::InputDouble("x",&v[0],0,0,"%.8g");
    changed|=ImGui::InputDouble("y",&v[1],0,0,"%.8g");
    changed|=ImGui::InputDouble("z",&v[2],0,0,"%.8g");
    ImGui::PopID();
    return changed;
}
static bool quatEdit(const char* label, Quat& q) {
    ImGui::SeparatorText(label);
    ImGui::PushID(label);
    bool changed=ImGui::InputDouble("s",&q.s,0,0,"%.8g");
    changed|=ImGui::InputDouble("x",&q.x,0,0,"%.8g");
    changed|=ImGui::InputDouble("y",&q.y,0,0,"%.8g");
    changed|=ImGui::InputDouble("z",&q.z,0,0,"%.8g");
    ImGui::PopID();
    return changed;
}
static bool matrixTable(const char* label, Matrix3x3& matrix, bool editable) {
    ImGui::SeparatorText(label);
    bool changed=false;
    if (ImGui::BeginTable(label,3,ImGuiTableFlags_Borders)) {
        for (int i=0;i<3;++i) {
            ImGui::TableNextRow();
            for (int j=0;j<3;++j) {
                ImGui::TableSetColumnIndex(j);
                if (editable) {
                    ImGui::PushID(i*3+j);
                    ImGui::SetNextItemWidth(-1);
                    changed|=ImGui::InputDouble("##value",&matrix.At(i,j),0,0,"%.8g");
                    ImGui::PopID();
                } else ImGui::Text("%.9g",matrix.At(i,j));
            }
        }
        ImGui::EndTable();
    }
    return changed;
}
static bool angleEdit(const char* label, double& angle) {
    double degrees=angle*180/Pi;
    if (!ImGui::InputDouble(label,&degrees,0,0,"%.8g")) return false;
    angle=degrees*Pi/180;
    return true;
}
static void vectorText(const char* label, const Vec3& v) {
    ImGui::Text("%s = (%.9g, %.9g, %.9g)",label,v[0],v[1],v[2]);
}
static void projection(const Vec3& before, const Vec3& after) {
    if (!isFinite(before) || !isFinite(after)) return;
    ImGui::SeparatorText("Vector projection (oblique view)");
    const ImVec2 pos=ImGui::GetCursorScreenPos();
    const float width=std::max(100.0f,ImGui::GetContentRegionAvail().x),height=220;
    ImGui::InvisibleButton("vectors",ImVec2(width,height));
    ImDrawList* draw=ImGui::GetWindowDrawList();
    const ImVec2 center(pos.x+width/2,pos.y+height/2);
    const double extent=std::max({1.0,std::abs(before[0]),std::abs(before[1]),std::abs(before[2]),
                                std::abs(after[0]),std::abs(after[1]),std::abs(after[2])});
    const float scale=static_cast<float>(std::min(width*0.25f,height*0.3f)/extent);
    auto endpoint=[&](const Vec3& v) { return ImVec2(center.x+scale*static_cast<float>(v[0]-0.55*v[2]),
                                                   center.y-scale*static_cast<float>(v[1]-0.35*v[2])); };
    const ImU32 axisColor=IM_COL32(110,115,125,255);
    draw->AddLine(center,endpoint({extent,0,0}),axisColor);
    draw->AddLine(center,endpoint({0,extent,0}),axisColor);
    draw->AddLine(center,endpoint({0,0,extent}),axisColor);
    draw->AddText(endpoint({extent,0,0}),axisColor,"x");
    draw->AddText(endpoint({0,extent,0}),axisColor,"y");
    draw->AddText(endpoint({0,0,extent}),axisColor,"z");
    draw->AddLine(center,endpoint(before),IM_COL32(85,170,255,255),3);
    draw->AddCircleFilled(endpoint(before),4,IM_COL32(85,170,255,255));
    draw->AddLine(center,endpoint(after),IM_COL32(255,180,75,255),3);
    draw->AddCircleFilled(endpoint(after),4,IM_COL32(255,180,75,255));
    ImGui::TextUnformatted("Blue: original vector. Orange: rotated vector.");
}

int main(int, char**) {
    if (!SDL_Init(SDL_INIT_VIDEO)) { std::fprintf(stderr, "%s\n", SDL_GetError()); return 1; }
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    const char* glsl = "#version 150";
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    const char* glsl = "#version 330";
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_Window* window = SDL_CreateWindow("Lab 2 - Mathematics II - 2026", 1200, 850,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    SDL_GLContext context = window ? SDL_GL_CreateContext(window) : nullptr;
    if (!context || !SDL_GL_MakeCurrent(window, context)) {
        std::fprintf(stderr, "%s\n", SDL_GetError());
        if (context) SDL_GL_DestroyContext(context);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit(); return 1;
    }
    SDL_GL_SetSwapInterval(1);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetIO().IniFilename = nullptr;

    ImGui::GetIO().Fonts->AddFontDefaultBitmap();
    ImGui::StyleColorsDark();
    const ImGuiStyle baseStyle = ImGui::GetStyle();
    float currentScale = 0;
    if (!ImGui_ImplSDL3_InitForOpenGL(window, context) || !ImGui_ImplOpenGL3_Init(glsl)) {
        std::fprintf(stderr, "Cannot initialize the ImGui backends\n");
        ImGui::DestroyContext(); SDL_GL_DestroyContext(context);
        SDL_DestroyWindow(window); SDL_Quit(); return 1;
    }

    Input input=exampleInput(0);
    Report report;
    bool hasReport=false,running=true;
    int selected=0,example=0;
    std::string error;
    const char* tasks[]={"Check IsRotation", "Axis and angle -> matrix", "Axis and angle -> quaternion",
        "Matrix -> axis and angle", "Matrix -> quaternion", "Matrix -> Euler ZYX",
        "Euler ZYX -> matrix", "Product q1 * q2", "Quaternion -> axis and angle",
        "Quaternion -> matrix", "Rotate vector with matrix", "Rotate vector with quaternion"};
    const char* examples[]={"45-degree rotation about z", "Identity (0 degrees)",
        "180 degrees, axis (0,1,-1)", "Near 180 degrees", "Gimbal lock +90 degrees",
        "Gimbal lock -90 degrees", "Near gimbal lock", "Reflection (invalid matrix)",
        "Zero axis and quaternion"};
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED
                && event.window.windowID == SDL_GetWindowID(window))) running = false;
        }

        float scale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(window));
        if (!std::isfinite(scale) || scale <= 0) scale = 1;

        scale = std::max(1.0f, std::round(scale));
        if (std::abs(scale - currentScale) > 0.001f) {
            ImGui::GetStyle() = baseStyle;
            ImGui::GetStyle().ScaleAllSizes(scale);
            ImGui::GetStyle().FontScaleDpi = scale;
            currentScale = scale;
        }
        ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplSDL3_NewFrame(); ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGui::Begin("Laboratory 2",nullptr,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        ImGui::TextUnformatted("Laboratory 2 · Rotations · Autumn 2026");
        ImGui::TextUnformatted("UI angles are in degrees; exercise functions use radians.");
        bool changed=ImGui::Combo("Operation",&selected,tasks,IM_ARRAYSIZE(tasks));
        ImGui::Combo("Example",&example,examples,IM_ARRAYSIZE(examples));
        ImGui::SameLine();
        if (ImGui::Button("Load example")) { input=exampleInput(example); changed=true; }
        const Task task=static_cast<Task>(selected);
        const bool axisInput=task==Task::AxisMatrix || task==Task::AxisQuat;
        const bool matrixInput=task==Task::MatrixAxis || task==Task::MatrixQuat || task==Task::MatrixEuler
            || task==Task::MatrixVector || task==Task::IsRotation;
        const bool quatInput=task==Task::QuatProduct || task==Task::QuatAxis || task==Task::QuatMatrix || task==Task::QuatVector;
        const bool vectorInput=task==Task::MatrixVector || task==Task::QuatVector;
        if (ImGui::BeginTable("panels",2,ImGuiTableFlags_Resizable)) {
            ImGui::TableNextColumn();
            ImGui::BeginChild("inputs",ImVec2(0,0));
            ImGui::SeparatorText("Data");
            if (axisInput) {
                changed|=vectorEdit("Axis u (normalized)",input.axis);
                changed|=angleEdit("Angle (degrees)",input.angle);
            }
            if (matrixInput) changed|=matrixTable("Matrix R",input.matrix,true);
            if (quatInput) changed|=quatEdit("Quaternion q1 = (s,x,y,z)",input.first);
            if (task==Task::QuatProduct) changed|=quatEdit("Quaternion q2",input.second);
            if (vectorInput) changed|=vectorEdit("Vector v",input.vector);
            if (task==Task::EulerMatrix) {
                changed|=angleEdit("Yaw (psi, Z)",input.yaw);
                changed|=angleEdit("Pitch (theta, Y local)",input.pitch);
                changed|=angleEdit("Roll (phi, X local)",input.roll);
            }
            ImGui::TextWrapped("Intrinsic ZYX: R = Rz(yaw) Ry(pitch) Rx(roll). Column vectors; local frame -> world.");
            ImGui::EndChild();
            ImGui::TableNextColumn();
            ImGui::BeginChild("results",ImVec2(0,0));
            if (changed) { hasReport=false; error.clear(); }
            if (ImGui::Button("Run operation")) {
                try { report=runTask(task,input); hasReport=true; error.clear(); }
                catch (const std::exception& exception) { error=exception.what(); hasReport=false; }
            }
            if (!error.empty()) ImGui::TextWrapped("Error: %s",error.c_str());
            if (hasReport) {
                ImGui::TextColored(report.correct ? ImVec4(0.4f,0.9f,0.5f,1) : ImVec4(1,0.45f,0.35f,1),
                    "Result: %s",report.correct ? "CORRECT" : "REVIEW");
                if (!report.detail.empty()) ImGui::TextWrapped("%s",report.detail.c_str());
                if (report.completed) {
                    ImGui::Text("Time: %.6f ms (informational)",report.milliseconds);
                    ImGui::Text("Error: %.3e; tolerance: 5e-6",report.error);
                    if (report.countsApplicable) {
                        const auto& op=report.operations;
                        ImGui::Text("+ %llu | - %llu | * %llu | / %llu",
                            static_cast<unsigned long long>(op.add),static_cast<unsigned long long>(op.subtract),
                            static_cast<unsigned long long>(op.multiply),static_cast<unsigned long long>(op.divide));
                        ImGui::TextWrapped("Typical operation count: %s. Other algorithms may have different costs: justify them.",
                            report.countsExpected ? "MATCHES" : "DIFFERENT");
                    }
                    const bool matrixResult=task==Task::AxisMatrix || task==Task::EulerMatrix || task==Task::QuatMatrix;
                    const bool quatResult=task==Task::AxisQuat || task==Task::MatrixQuat || task==Task::QuatProduct;
                    if (matrixResult) {
                        matrixTable("Result R",report.matrix,false);
                        if (ImGui::Button("Copy R to input")) input.matrix=report.matrix;
                    }
                    if (quatResult) {
                        ImGui::Text("q = (%.9g, %.9g, %.9g, %.9g)",report.quat.s,report.quat.x,report.quat.y,report.quat.z);
                        if (ImGui::Button("Copy q to q1")) { input.first=report.quat; hasReport=false; }
                    }
                    if (task==Task::MatrixAxis || task==Task::QuatAxis) {
                        vectorText("u",report.axis);
                        ImGui::Text("Angle: %.9g degrees",report.angle*180/Pi);
                    }
                    if (task==Task::MatrixEuler)
                        ImGui::Text("Yaw: %.9g | Pitch: %.9g | Roll: %.9g degrees",
                            report.yaw*180/Pi,report.pitch*180/Pi,report.roll*180/Pi);
                    if (task==Task::IsRotation) ImGui::Text("IsRotation: %s",report.rotation ? "true" : "false");
                    if (vectorInput) { vectorText("v'",report.vector); projection(input.vector,report.vector); }
                }
            }
            ImGui::EndChild();
            ImGui::EndTable();
        }
        ImGui::End(); ImGui::Render();
        int width, height; SDL_GetWindowSizeInPixels(window, &width, &height);
        glViewport(0, 0, width, height); glClearColor(0.10f, 0.11f, 0.14f, 1); glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); SDL_GL_SwapWindow(window);
    }
    ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplSDL3_Shutdown(); ImGui::DestroyContext();
    SDL_GL_DestroyContext(context); SDL_DestroyWindow(window); SDL_Quit();
    return 0;
}
