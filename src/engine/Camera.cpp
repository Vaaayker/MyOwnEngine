#include "Camera.hpp"
#include <cmath>

void Camera::Create(SDL_Window* window)
{
    pointView = {6.0f, 1.0f, 3.5f};
    pointUP = {5.0f, 2.0f, 3.5f};
    position = {5.0f, 1.0f, 3.5f};

    directView = Vec3::Normalize(Point3D::MakeVec(position, pointView));
    directUP = Vec3::Normalize(Point3D::MakeVec(position, pointUP)); 
    directWidth = Vec3::Cross(directView, directUP);

    FieldOfViewAngle = 90.0f;
    NearPlane = 0.1f;
    FarPlane = 10.0f;

    int width = 0,height = 0;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    Aspect = static_cast<float>(width) / static_cast<float>(height);

    SetViewMatrix();
    SetProjectionMatrix();
}

void Camera::SetViewMatrix()
{
    Vec3 widthDirect = {10.0f, 0.0f, 0.0f};
    Vec3 heightDirect = {0.0f, 5.0f, 0.0f};
    Vec3 depthDirect = {0.0f, 0.0f, 7.0f};

    float tempViewMatrix[4][4] = {
        {directWidth.x, directUP.x, directView.x, Vec3::MyDot(directWidth, widthDirect)},
        {directWidth.y, directUP.y, directView.y, Vec3::MyDot(directUP, heightDirect)},
        {directWidth.z, directUP.z, directView.z, Vec3::MyDot(directView, depthDirect)},
        {0.0f, 0.0f, 0.0f, 0.0f}
    };

    for(int cols = 0; cols < 4; cols++)
    {
        for(int rows = 0; rows < 4; rows++)
        {
            viewMatrix.mat[cols][rows] = tempViewMatrix[cols][rows];
        }
    }
}

void Camera::SetProjectionMatrix()
{
    constexpr float PI = 3.14159265358979323846f;
    const float fieldOfViewRadians = FieldOfViewAngle * PI / 180.0f;

    projectionMatrix.mat[0][0] = 1 / (Aspect * std::tan(fieldOfViewRadians / 2)); 
    projectionMatrix.mat[1][1] = 1 / std::tan(fieldOfViewRadians / 2); 
    projectionMatrix.mat[2][2] = FarPlane / (FarPlane - NearPlane); 
    projectionMatrix.mat[2][3] = (NearPlane * FarPlane) / (NearPlane - FarPlane); 
    projectionMatrix.mat[3][2] = 1.0f; 
}
