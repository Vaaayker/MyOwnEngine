#pragma once
#include "Vec3.hpp"
#include "Point3D.hpp"
#include "mat4.hpp"

class Camera
{
private:
    // точки
    Point3D pointView;
    Point3D pointUP;
    Point3D position;

    // напрямки
    Vec3 directView;
    Vec3 directUP;
    Vec3 directWidth;

    // матриці
    mat4 viewMatrix; // погляду
    mat4 projectionMatrix; // проекції

    // елементи для матриці проекції
    float FieldOfViewAngle; // кут огляду
    float NearPlane; // відстань від камери до проекційної площини
    float FarPlane; // відстань від камери до дальної площини
    float Aspect; // відношення ширини до висоти

public:
    Camera();
    void SetViewMatrix();
    void SetProjectionMatrix();
};