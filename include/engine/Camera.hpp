#pragma once
#include "Vec3.hpp"
#include "Point3D.hpp"
#include "mat4.hpp"
#include <SDL3/SDL_vulkan.h>

/**
 * @brief Stores the camera state and creates its view and projection matrices.
 *
 * Uses the camera position, view direction, up direction, field of view,
 * aspect ratio, and clipping planes to calculate the camera matrices.
 */

class Camera
{
public:
    Camera() = default;
    void Create(SDL_Window* window);
    
private:
    // points
    Point3D pointView{};
    Point3D pointUP{};
    Point3D position{};

    // directions
    Vec3 directView{};
    Vec3 directUP{};
    Vec3 directWidth{};

    // matrices
    mat4 viewMatrix{}; // matrix of view
    mat4 projectionMatrix{}; // matrix of projection

    // elements for matrix of projection
    float FieldOfViewAngle; 
    float NearPlane; 
    float FarPlane; 
    float Aspect; 

private:
    void SetViewMatrix();
    void SetProjectionMatrix();
};