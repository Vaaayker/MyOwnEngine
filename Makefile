SRC = src/engine/*.cpp \
      src/engine/math/*.cpp

INCLUDE = -I include/ \
		 -I include/math	\
		 -I /Users/bereznakmaksim/VulkanSDK/1.4.350.0/macOS/include

FLAGS = -F external -framework SDL3 \
			-L /Users/bereznakmaksim/VulkanSDK/1.4.350.0/macOS/lib -lvulkan

RPATH = -Wl,-rpath,/Users/bereznakmaksim/Documents/devProjects/MyOwnEngine/external \
			-Wl,-rpath,/Users/bereznakmaksim/VulkanSDK/1.4.350.0/macOS/lib

all:
	clang++ $(SRC) $(INCLUDE)  $(FLAGS) $(RPATH) -o bin/main

