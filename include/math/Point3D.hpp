#pragma once
#include "Vec3.hpp"

/**
 * @brief Represents a point in 3D space and provides vector creation between points.
 */

struct Point3D
{
    float x;
    float y;
    float z;

    Point3D();
    Point3D(float x, float y, float z);
    static Vec3 MakeVec(Point3D A, Point3D B);
};