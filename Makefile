SRC = src/engine/main.cpp
FLAGS = -Wl,-rpath
RPATH = /Users/bereznakmaksim/Documents/devProjects/MyOwnEngine/external
IFLAGS = include/

all:
	clang++ $(SRC) -I$(IFLAGS) -Fexternal -framework SDL3 $(FLAGS) $(RPATH) -o bin/main

