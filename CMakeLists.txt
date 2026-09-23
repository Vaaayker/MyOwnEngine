cmake_minimum_required(VERSION 4.4.3)

project(MyOwnEngine)

add_executable(MyOwnEngine
    src/engine/Camera.cpp
    src/engine/Mesh.cpp
    src/engine/Room.cpp
    src/math/mat4.cpp
    src/math/Point3D.cpp
    src/math/Vec3.cpp
    src/math/Vec4.cpp
    src/vulkan/CommandPool.cpp
    src/vulkan/DebugMessanger.cpp
    src/vulkan/GraphicsPipeline.cpp
    src/vulkan/Renderer.cpp
    src/vulkan/Swapchain.cpp
    src/vulkan/SyncObjects.cpp
    src/vulkan/VulkanContext.cpp
    src/main.cpp)

target_include_directories(MyOwnEngine
    PRIVATE
    include/engine
    include/math
    include/vulkan)

target_compile_features(MyOwnEngine 
    PRIVATE 
    cxx_std_17)

target_compile_definitions(MyOwnEngine
    PRIVATE
        VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
)

target_compile_options(MyOwnEngine
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
    -Werror
)

find_package(Vulkan)
target_link_libraries(MyOwnEngine 
    PRIVATE 
    Vulkan::Vulkan)

find_package(SDL3 REQUIRED)
target_link_libraries(MyOwnEngine
    PRIVATE
    SDL3::SDL3)

find_package(Vulkan REQUIRED COMPONENTS glslc)
add_custom_command(
    OUTPUT 
    shaders/compiled/triangle.vert.spv 
    shaders/compiled/triangle.frag.spv

    DEPENDS
    triangle.frag
    triangle.vert

    COMMAND
    glslc shaders/triangle.vert -o shaders/compiled/triangle.vert.spv 

    COMMAND
	glslc shaders/triangle.frag -o shaders/compiled/triangle.frag.spv
)
