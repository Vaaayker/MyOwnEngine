#pragma once
#include "Vec3.hpp"
#include "Point3D.hpp"

class Camera
{
private:
    Point3D pointView;
    Point3D pointUP;
    Point3D position;
    Vec3 directView;
    Vec3 directUP;
    Vec3 directWidth;
    float viewMatrix[4][4];

public:
    Camera();
    void SetViewMatrix();
};