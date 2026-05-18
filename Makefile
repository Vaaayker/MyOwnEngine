SRC = src/engine/*.cpp \
      src/engine/math/*.cpp
FLAGS = -Fexternal -framework SDL3 
RPATH = -Wl,-rpath,/Users/bereznakmaksim/Documents/devProjects/MyOwnEngine/external
IFLAGS = -Iinclude/ \
		 -Iinclude/math


all:
	clang++ $(SRC) $(IFLAGS)  $(FLAGS) $(RPATH) -o bin/main

