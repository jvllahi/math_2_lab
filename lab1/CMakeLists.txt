cmake_minimum_required(VERSION 3.24)
project(Lab1 LANGUAGES C CXX)
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build configuration" FORCE)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(USE "assignment" CACHE STRING "Implementation to use: assignment or solution")
set_property(CACHE USE PROPERTY STRINGS assignment solution)
if(USE STREQUAL "assignment")
    set(LAB1_IMPL_DIR assignment)
    set(LAB1_LINALG_SOURCE ${LAB1_IMPL_DIR}/LinAlgebra.cpp)
elseif(USE STREQUAL "solution")
    set(LAB1_IMPL_DIR solution)
    set(LAB1_LINALG_SOURCE ${LAB1_IMPL_DIR}/LinAlgebra.cpp)
else()
    message(FATAL_ERROR "Invalid USE='${USE}'. Expected 'assignment' or 'solution'.")
endif()

add_library(labcore ${LAB1_LINALG_SOURCE} ${LAB1_IMPL_DIR}/Solver.cpp
    helpers/Numerics.cpp helpers/Dataset.cpp helpers/Validation.cpp)
target_include_directories(labcore PUBLIC ${LAB1_IMPL_DIR} helpers)
if(MSVC)
    target_compile_options(labcore PUBLIC /utf-8)
    target_compile_options(labcore PRIVATE /W4)
else()
    target_compile_options(labcore PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(LAB1_BUILD_TESTS "Build unit tests for the selected implementation" OFF)
if(LAB1_BUILD_TESTS)
    enable_testing()
    add_executable(lab1_tests
        tests/main.cpp
        tests/TestLinearAlgebra.cpp
        tests/TestSolver.cpp)
    target_link_libraries(lab1_tests PRIVATE labcore)
    add_test(NAME lab1_tests COMMAND lab1_tests)
endif()

option(LAB1_BUILD_UI "Build the SDL3 / OpenGL / Dear ImGui interface" ON)
if(NOT LAB1_BUILD_UI)
    return()
endif()
include(FetchContent)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3 URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-3.2.30.tar.gz
    URL_HASH SHA256=8bfb8f70a72f216dff66cde64b9d7b863d5e8aa46be7f2fd400d7f1f9baafe76
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_Declare(imgui URL https://github.com/ocornut/imgui/archive/refs/tags/v1.92.6.tar.gz
    URL_HASH SHA256=5b17c01f69545bde732b14936d89ce0f508adb83e8b56fa82448371845172bc3
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(SDL3 imgui)
find_package(OpenGL REQUIRED)
add_library(imgui STATIC ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC SDL3::SDL3 OpenGL::GL)
add_executable(lab1 app/main.cpp ui/Actions.cpp ui/Widgets.cpp ui/View.cpp)
target_include_directories(lab1 PRIVATE ui)
target_link_libraries(lab1 PRIVATE labcore imgui)
set_target_properties(lab1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
foreach(config Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${config} upper)
    set_target_properties(lab1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${upper} "${CMAKE_BINARY_DIR}/bin")
endforeach()
add_custom_command(TARGET lab1 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/datasets" "$<TARGET_FILE_DIR:lab1>/datasets")
install(TARGETS lab1 RUNTIME DESTINATION .)
install(DIRECTORY datasets DESTINATION .)
