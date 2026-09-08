#pragma once

/**
 * @brief Represents a four-component vector used for homogeneous coordinates.
 *
 * Can create a direction vector with w = 0 or a point vector with w = 1
 * from a three-component vector.
 */

#include "Vec3.hpp"

struct Vec4
{
    float x;
    float y;
    float z;
    float w;

    Vec4();
    Vec4(float x, float y, float z, float w);

    Vec4 makeDirectionVec4(Vec3 vec3);
    Vec4 makePointVec4(Vec3 vec3);
};