SRC = src/engine/*.cpp \
      src/math/*.cpp \
	  src/vulkan/*.cpp \
	  src/main.cpp

INCLUDE = -I include/engine \
		 -I include/math \
		 -I include/vulkan \
		 -I /Users/bereznakmaksim/VulkanSDK/1.4.350.0/macOS/include

FLAGS = -F external -framework SDL3 \
			-L /Users/bereznakmaksim/VulkanSDK/1.4.350.0/macOS/lib -lvulkan

RPATH = -Wl,-rpath,/Users/bereznakmaksim/Documents/devProjects/MyOwnEngine/external \
			-Wl,-rpath,/Users/bereznakmaksim/VulkanSDK/1.4.350.0/macOS/lib

WARNING_FLAGS = -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror

DEBUG_FLAGS = -g -O0

RELEASE_FLAGS = -O2 -DNDEBUG

OUTPUT_DEBUG = -o bin/debug/main
OUTPUT_RELEASE = -o bin/release/main

LOADER = -DVULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1

debug:
	clang++ -std=c++17 $(WARNING_FLAGS) $(SRC) $(INCLUDE) $(DEBUG_FLAGS) $(FLAGS) $(RPATH)  $(LOADER) $(OUTPUT_DEBUG) 
	glslc shaders/triangle.vert -o shaders/compiled/triangle.vert.spv 
	glslc shaders/triangle.frag -o shaders/compiled/triangle.frag.spv 

release:
	clang++ -std=c++17 $(WARNING_FLAGS) $(SRC) $(INCLUDE) $(RELEASE_FLAGS) $(FLAGS) $(RPATH) $(LOADER) $(OUTPUT_RELEASE) 
	glslc shaders/triangle.vert -o shaders/compiled/triangle.vert.spv 
	glslc shaders/triangle.frag -o shaders/compiled/triangle.frag.spv

clean-debug:
	rm -rf build/debug

clean-release:
	rm -rf build/release



