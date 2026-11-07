cmake_minimum_required(VERSION 3.24)
project(Lab2 LANGUAGES C CXX)
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build configuration" FORCE)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
foreach(config Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${config} upper)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${upper} "${CMAKE_BINARY_DIR}/bin")
endforeach()

set(USE "assignment" CACHE STRING "Implementation to use: assignment or solution")
set_property(CACHE USE PROPERTY STRINGS assignment solution)
if(USE STREQUAL "assignment" OR USE STREQUAL "solution")
    set(LAB2_IMPL_DIR ${USE})
else()
    message(FATAL_ERROR "Invalid USE='${USE}'. Expected 'assignment' or 'solution'.")
endif()

foreach(file LinAlgebra.cpp LinAlgebra.hpp)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/algebra/${file}")
        message(FATAL_ERROR
            "Missing algebra/${file}. Copy your completed lab 1 LinAlgebra.cpp and "
            "LinAlgebra.hpp into lab2/algebra/ before configuring. See README.md.")
    endif()
endforeach()

set(LAB_SOURCES ${LAB2_IMPL_DIR}/Matrix3x3.cpp ${LAB2_IMPL_DIR}/Quat.cpp
    ${LAB2_IMPL_DIR}/Matrix3x3.hpp ${LAB2_IMPL_DIR}/Quat.hpp
    algebra/LinAlgebra.cpp algebra/LinAlgebra.hpp
    helpers/Numerics.cpp helpers/Numerics.hpp
    helpers/Geometry.cpp helpers/Geometry.hpp
    helpers/Validation.cpp helpers/Validation.hpp)
add_library(labcore ${LAB_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${LAB_SOURCES})
target_include_directories(labcore PUBLIC ${LAB2_IMPL_DIR} algebra helpers)
if(MSVC)
    target_compile_options(labcore PUBLIC /utf-8)
    target_compile_options(labcore PRIVATE /W4)
else()
    target_compile_options(labcore PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(LAB2_BUILD_TESTS "Build unit tests for the selected implementation" OFF)
if(LAB2_BUILD_TESTS)
    find_package(GTest CONFIG QUIET)
    if(NOT GTest_FOUND)
        include(FetchContent)
        FetchContent_Declare(googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()
    enable_testing()
    add_executable(lab2_tests tests/TestGeometry.cpp tests/TestRotations.cpp tests/TestValidation.cpp)
    target_link_libraries(lab2_tests PRIVATE labcore GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(lab2_tests)
endif()

option(LAB2_BUILD_UI "Build the SDL3 / OpenGL / Dear ImGui interface" ON)
if(NOT LAB2_BUILD_UI)
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
add_executable(lab2 app/main_app.cpp)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT lab2)
target_link_libraries(lab2 PRIVATE labcore imgui)
install(TARGETS lab2 RUNTIME DESTINATION .)
