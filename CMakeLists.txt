cmake_minimum_required(VERSION 3.22.1)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH};")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(vcpkg_setup_toolchain.cmake)
vcpkg_setup_toolchain()

project(voxel-experiment VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#vcpkg_add_dependencies(
    #"argparse"
    #"glew"
    #"glm"
    #"opengl"
    #"sdl2"
    #"jsoncpp"
    #)

add_definitions(-D_USE_MATH_DEFINES)

find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(argparse CONFIG REQUIRED)
#find_package(jsoncpp CONFIG REQUIRED)

set(${PROJECT_NAME}_source
    "src/app.cpp"
    "src/axismaterial.cpp"
    "src/axisgeometry.cpp"
    "src/axis.cpp"
    "src/context.cpp"
    "src/main.cpp"
    "src/shader.cpp"
    "src/program.cpp"
    "src/window.cpp"
    "src/uicontext.cpp"
    "src/mainwindow.cpp"
    "src/cameraui.cpp"
    "src/appcamera.cpp"
    "src/renderer.cpp"
    #"src/voxelchunk.cpp"
    )

set(${PROJECT_NAME}_include
    "src/log.hpp"
    "src/app.hpp"
    "src/axismaterial.hpp"
    "src/axisgeometry.hpp"
    "src/axis.hpp"
    "src/context.hpp"
    "src/shader.hpp"
    "src/program.hpp"
    "src/geometry.hpp"
    "src/material.hpp"
    "src/window.hpp"
    "src/renderable.hpp"
    "src/camera.hpp"
    "src/uicontext.hpp"
    "src/ui.hpp"
    "src/mainwindow.hpp"
    "src/cameraui.hpp"
    "src/appcamera.hpp"
    "src/node.hpp"
    "src/renderer.hpp"
    #"src/voxelchunk.hpp"
    "src/axisshaders.h"
    )

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_include}
    ${${PROJECT_NAME}_source}
    )

#target_include_directories(${PROJECT_NAME}
#)

target_link_libraries(${PROJECT_NAME}
    PRIVATE argparse::argparse
    PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    PRIVATE OpenGL::GL
    PRIVATE glm::glm
    PRIVATE GLEW::GLEW
    PRIVATE imgui::imgui
    #PRIVATE JsonCpp::JsonCpp
    )

