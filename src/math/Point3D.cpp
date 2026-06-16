#include "Point3D.hpp"

Vec3 Point3D::MakeVec(Point3D A, Point3D B)
{
    Vec3 vec3 = {B.x - A.x, B.y - A.y, B.z - A.z};
    return vec3;
}