#include "Camera.hpp"

Camera::Camera()
{
    pointView = {6.0f, 1.0f, 3.5f};
    pointUP = {5.0f, 2.0f, 3.5f};
    position = {5.0f, 1.0f, 3.5f};
    directView = Vec3::Normalize(Point3D::MakeVec(position, pointView));
    directUP = Vec3::Normalize(Point3D::MakeVec(position, pointUP)); 
    directWidth = Vec3::Cross(directView, directUP);
}

void Camera::SetViewMatrix()
{
    Vec3 widthDirect = {10.0f, 0, 0};
    Vec3 heightDirect = {0, 5.0f, 0};
    Vec3 depthDirect = {0, 0, 7.0f};

    float tempViewMatrix[4][4] = {
        {directWidth.x, directUP.x, directView.x, Vec3::MyDot(directWidth, widthDirect)},
        {directWidth.y, directUP.y, directView.y, Vec3::MyDot(directUP, heightDirect)},
        {directWidth.z, directUP.z, directView.z, Vec3::MyDot(directView, depthDirect)},
        {0, 0, 0, 0}
    };

    for(int cols = 0; cols < 4; cols++)
    {
        for(int rows = 0; rows < 4; rows++)
        {
            viewMatrix.mat[cols][rows] = tempViewMatrix[cols][rows];
        }
    }
}