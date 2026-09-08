#pragma once
#include "mat4.hpp"

/**
 * @brief Stores the room dimensions and its model matrix.
 *
 * Initializes the room width, height, depth, and corresponding scale values
 * in the model matrix.
 */

class Room
{
public:
    Room();
    
private:
    float width;
    float height;
    float depth;
    mat4 modelMatrix{};
};