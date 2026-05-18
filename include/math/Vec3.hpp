#pragma once

struct Vec3
{
    float x;
    float y; 
    float z; 

    static Vec3 Normalize(Vec3 vec3);
    static Vec3 Cross(Vec3 vecA, Vec3 vecB);
    static float MyDot(Vec3 vecA, Vec3 vecB);
};
