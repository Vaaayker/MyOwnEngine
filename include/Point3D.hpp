#pragma once
#include "Vec3.hpp"

struct Point3D
{
    float x;
    float y;
    float z;

    static Vec3 MakeVec(Point3D A, Point3D B);
};