cmake_minimum_required(VERSION 3.22.1)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH};")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(vcpkg_dependencies.cmake)
vcpkg_setup_toolchain()

project(shader-viewer-4 VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

vcpkg_add_dependencies(
    "argparse"
    "glew"
    "glm"
    "opengl"
    "sdl2"
    "jsoncpp"
    )

add_definitions(-D_USE_MATH_DEFINES)

find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

set(${PROJECT_NAME}_source
    app.cpp
    context.cpp
    main.cpp
    shader.cpp
    program.cpp
    sandbox.cpp
    sandboxmaterial.cpp
    planegeometry.cpp
    window.cpp
    )

set(${PROJECT_NAME}_include
    log.hpp
    app.hpp
    context.hpp
    shader.hpp
    program.hpp
    sandbox.hpp
    material.hpp
    sandboxmaterial.hpp
    planegeometry.hpp
    window.hpp
    renderable.hpp
    )

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_include}
    ${${PROJECT_NAME}_source}
    )

#target_include_directories(${PROJECT_NAME}
#)

target_link_libraries(${PROJECT_NAME}
    PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    PRIVATE OpenGL::GL
    PRIVATE glm::glm
    PRIVATE GLEW::GLEW
    PRIVATE JsonCpp::JsonCpp
    )

