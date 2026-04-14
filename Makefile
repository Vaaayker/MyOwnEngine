SRC = src/engine/main.cpp
FLAGS = -Wl,-rpath
RPATH = /Users/bereznakmaksim/Documents/devProjects/MyOwnEngine/external

all:
	clang++ $(SRC) -Fexternal -framework SDL3 $(FLAGS) $(RPATH) -o bin/main

