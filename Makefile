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

debug:
	clang++ -std=c++17 $(SRC) $(INCLUDE)  -g -O0 $(FLAGS) $(RPATH) -o bin/main -DVULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1

release:
	clang++ -std=c++17 $(SRC) $(INCLUDE)  -O2  -DNDEBUG $(FLAGS) $(RPATH) -o bin/main -DVULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1

