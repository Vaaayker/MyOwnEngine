#pragma once

/**
 * @brief Represents a three-dimensional vector.
 *
 * Stores the x, y, and z components and provides basic vector operations,
 * including normalization, cross product, and dot product.
 */

struct Vec3
{
    float x;
    float y; 
    float z; 

    Vec3();
    Vec3(float x, float y, float z);

    static Vec3 Normalize(Vec3 vec3);
    static Vec3 Cross(Vec3 vecA, Vec3 vecB);
    static float MyDot(Vec3 vecA, Vec3 vecB);
};
