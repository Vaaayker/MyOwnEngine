# MyOwnEngine

## Overview
MyOwnEngine is a learning-oriented low-level rendering engine written in C++ and Vulkan.
The project focuses on understanding low-level graphics programming, C++ fundamentals, rendering architecture, resource management, synchronization, and performance.
The engine is developed incrementally by implementing increasingly complex rendering systems and solving real-world technical problems.

## Features
What the project can do or what functionality it provides:
- C++17
- Vulkan SDK
- SDL3
- Vulkan validation layers
- Swapchain recreation
- Synchronization with multiple frames in flight
- SPIR-V shader compilation
- CMake + Ninja build system

## Requirements
What you need to install or have available to build and run the project:
- C++17 compatible compiler
- CMake
- Ninja
- Vulkan SDK
- SDL3

## Build

### Debug
Configure and make a Debug build:
```bash
cmake -S . -B build/debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja -C build/debug
```

### Release
Configure and make a Release build:
```bash
cmake -S . -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -C build/release
```

## Run
Open the target due to bash. For release version:
```bash
./build/release/MyOwnRelease
```
For debug version:
```bash
./build/debug/MyOwnRelease
```

## Project Structure

The project includes:
* `src/` — engine source code.
* `include/` — public headers.
* `shaders/` — GLSL shader source files.
* `external/` — external dependencies.
* `CMakeLists.txt` — CMake build configuration.
* `build/` — generated build files and compiled binaries; not tracked by Git.


## Dependencies

The project uses:
- Vulkan SDK
- SDL3
- CMake
- Ninja

External dependencies and the Vulkan SDK are not included in the repository.
They must be installed separately.
