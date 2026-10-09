#include "Actions.hpp"
#include "State.hpp"
#include "View.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

int main(int argc, char** argv) {
    // Initialize SDL video subsystem before creating any window or GL context.
    if (!SDL_Init(SDL_INIT_VIDEO)) { std::fprintf(stderr, "%s\n", SDL_GetError()); return 1; }
#ifdef __APPLE__
    // macOS commonly uses core profile 3.2 with GLSL 150.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    const char* glsl = "#version 150";
#else
    // Other platforms target core profile 3.3 with GLSL 330.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    const char* glsl = "#version 330";
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Create the main window and attach an OpenGL context.
    SDL_Window* window = SDL_CreateWindow("Lab 1 - Mathematics II - 2026", 1200, 850,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    SDL_GLContext context = window ? SDL_GL_CreateContext(window) : nullptr;
    if (!context || !SDL_GL_MakeCurrent(window, context)) {
        std::fprintf(stderr, "%s\n", SDL_GetError());
        if (context) SDL_GL_DestroyContext(context);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit(); return 1;
    }
    // Enable vsync to reduce tearing and unnecessary CPU/GPU usage.
    SDL_GL_SetSwapInterval(1);

    // Bootstrap Dear ImGui context and platform/renderer backends.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetIO().IniFilename = nullptr;

    ImGui::GetIO().Fonts->AddFontDefaultBitmap();
    ImGui::StyleColorsDark();
    const ImGuiStyle baseStyle = ImGui::GetStyle();
    float currentScale = 0;
    if (!ImGui_ImplSDL3_InitForOpenGL(window, context) || !ImGui_ImplOpenGL3_Init(glsl)) {
        std::fprintf(stderr, "Cannot initialize ImGui backends\n");
        ImGui::DestroyContext(); SDL_GL_DestroyContext(context);
        SDL_DestroyWindow(window); SDL_Quit(); return 1;
    }

    // Initialize app-level run state (datasets, defaults, optional CLI path).
    RunState state;
    initializeState(state, argc, argv);

    // Main loop: process events, build UI, render frame, present.
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED
                && event.window.windowID == SDL_GetWindowID(window))) running = false;
        }

            // Update UI scaling when display DPI/content scale changes.
        float scale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(window));
        if (!std::isfinite(scale) || scale <= 0) scale = 1;

        scale = std::max(1.0f, std::round(scale));
        if (std::abs(scale - currentScale) > 0.001f) {
            ImGui::GetStyle() = baseStyle;
            ImGui::GetStyle().ScaleAllSizes(scale);
            ImGui::GetStyle().FontScaleDpi = scale;
            currentScale = scale;
        }

        // Build and render one ImGui frame.
        ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplSDL3_NewFrame(); ImGui::NewFrame();
        renderWindow(state);
        ImGui::Render();
        int width, height; SDL_GetWindowSizeInPixels(window, &width, &height);
        glViewport(0, 0, width, height); glClearColor(0.10f, 0.11f, 0.14f, 1); glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); SDL_GL_SwapWindow(window);
    }

    // Shutdown in reverse order of initialization.
    ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplSDL3_Shutdown(); ImGui::DestroyContext();
    SDL_GL_DestroyContext(context); SDL_DestroyWindow(window); SDL_Quit();
    return 0;
}
