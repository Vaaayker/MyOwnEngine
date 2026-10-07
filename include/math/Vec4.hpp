#pragma once
#include <cstdint>

// forward declaration
struct Vec3;

/**
 * @brief Represents a four-component vector used for homogeneous coordinates.
 *
 * Can create a direction vector with w = 0 or a point vector with w = 1
 * from a three-component vector.
 */

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

    static Vec4 Normalize(Vec4 vec4);
    static Vec4 Cross(Vec4 vecA, Vec4 vecB);
    static float MyDot(Vec4 vecA, Vec4 vecB);

    float& operator[](std::uint32_t index);
    const float& operator[](std::uint32_t index) const;

    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;

};