#pragma once
#include "Vec3.hpp"

/**
 * @brief Represents a mesh vertex with position and normal vectors.
 */

struct Vertex
{
    Vec3 position{}; 
    Vec3 normal{};
};