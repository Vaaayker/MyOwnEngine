#include "Vec4.hpp"

Vec4 Vec4::makeDirectionVec4(Vec3 vec3)
{
    return {vec3.x, vec3.y, vec3.z, 0};
}

Vec4 Vec4::makePointVec4(Vec3 vec3)
{
    return {vec3.x, vec3.y, vec3.z, 1};
}