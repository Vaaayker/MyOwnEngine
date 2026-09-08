#include "Point3D.hpp"

Point3D::Point3D()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Point3D::Point3D(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3 Point3D::MakeVec(Point3D A, Point3D B)
{
    Vec3 vec3 = {B.x - A.x, B.y - A.y, B.z - A.z};
    return vec3;
}