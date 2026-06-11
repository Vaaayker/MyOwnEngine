#pragma once
#include "Vec3.hpp"

struct Vec4
{
    float x;
    float y;
    float z;
    float w;

    Vec4 makeDirectionVec4(Vec3 vec3);
    Vec4 makePointVec4(Vec3 vec3);
};