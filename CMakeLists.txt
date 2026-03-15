cmake_minimum_required(VERSION 3.22.1)

project(shader-viewer-4 VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-D_USE_MATH_DEFINES)

find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(argparse CONFIG REQUIRED)

set(${PROJECT_NAME}_sources
    "src/app.cpp"
    "src/context.cpp"
    "src/main.cpp"
    "src/shader.cpp"
    "src/program.cpp"
    "src/sandbox.cpp"
    "src/sandboxmaterial.cpp"
    "src/planegeometry.cpp"
    "src/window.cpp"
    "src/camera.cpp"
    "src/uicontext.cpp"
    "src/mainwindow.cpp"
    "src/cameraui.cpp"
)

set(${PROJECT_NAME}_headers
    "src/log.hpp"
    "src/app.hpp"
    "src/context.hpp"
    "src/shader.hpp"
    "src/program.hpp"
    "src/sandbox.hpp"
    "src/material.hpp"
    "src/sandboxmaterial.hpp"
    "src/planegeometry.hpp"
    "src/geometry.hpp"
    "src/window.hpp"
    "src/renderable.hpp"
    "src/camera.hpp"
    "src/uicontext.hpp"
    "src/ui.hpp"
    "src/mainwindow.hpp"
    "src/cameraui.hpp"
)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_sources}
    ${${PROJECT_NAME}_headers}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE argparse::argparse
    PRIVATE SDL3::SDL3
    PRIVATE OpenGL::GL
    PRIVATE glm::glm
    PRIVATE GLEW::GLEW
    PRIVATE imgui::imgui
)
