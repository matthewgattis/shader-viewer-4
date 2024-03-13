# shader-viewer-4

OpenGL based shader loading and rendering application, with built-in camera controls.

![](docs/example.jpg)

Resurected this project from incomplete source code from college. 

## Getting Started

### Prequisites
- CMake https://cmake.org/
- vcpkg https://github.com/microsoft/vcpkg
    - Follow Microsoft's instruction on setting up and boostrapping vcpkg.
    - Set the `VCPKG_ROOT` environment variable. Or set `-D VCPKG_ROOT=/path/to/vcpkg` on CMake command line.
    - https://learn.microsoft.com/en-us/vcpkg/get_started/get-started

### Building
```
$ git clone https://github.com/matthewgattis/shader-viewer-4.git
$ mkdir -p shader-viewer-4/build
$ cd shader-viewer-4/build
$ cmake -D CMAKE_BUILD_TYPE=Release ../
$ cmake --build .
```

### Running
- The shader view is currently setup to load a fragment shader called `fragment.glsl` from the current working directory.
- To run the shader pictured above, copy `fragment.glsl` to the build folder and run.
```
$ cp ../fragment.glsl ./
$ ./shader-viewer-4
``` 

### Controls
- Orbit controls:
    - Right mouse button: Orbit/rotate around pivot.
    - Middle mouse button: Pan view.
    - Scroll wheel: Zoom in/out.
- Free look:
    - Left mouse button: Free look.
    - W, A, S, D: Move.
    - Space: Ascend.
    - Left Shift: Descend.
- Both of these control schemes are active at the same time.
- Camera movement speed is influenced by zoom.
    - Zoom in to slow down.

## Future
- Integrate argument parsing with `argparse`.
- Would be nice to pull in ImGui for more controls.
- Pass camera projection matrix to shader.
    - Think I can use this to implement a double click to focus.
    - Similar to MeshLab.
- More example shaders.
- Refactor `app.cpp`.
