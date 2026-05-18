#include "mat4.hpp"
#pragma once

class Room
{
private:
    float width;
    float height;
    float depth;
    mat4 modelMatrix;

public:
    void AddElementModelMatrix();
    Room();
};