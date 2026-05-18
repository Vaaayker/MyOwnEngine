#pragma once
#include "Vec3.hpp"
#include "Point3D.hpp"
#include "mat4.hpp"

class Camera
{
private:
    Point3D pointView;
    Point3D pointUP;
    Point3D position;
    Vec3 directView;
    Vec3 directUP;
    Vec3 directWidth;
    mat4 viewMatrix;

public:
    Camera();
    void SetViewMatrix();
};