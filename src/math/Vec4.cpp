#include "Vec4.hpp"

Vec4::Vec4()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
}

Vec4::Vec4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vec4 Vec4::makeDirectionVec4(Vec3 vec3)
{
    return {vec3.x, vec3.y, vec3.z, 0};
}

Vec4 Vec4::makePointVec4(Vec3 vec3)
{
    return {vec3.x, vec3.y, vec3.z, 1};
}