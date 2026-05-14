#include "Vec3.hpp"
#include <math.h>

Vec3 Vec3::Normalize(Vec3 vec3)
{
	Vec3 ResultVec;
	float distance = sqrt(((vec3.x * vec3.x) + (vec3.y * vec3.y) + (vec3.y * vec3.y)));
	ResultVec.x = vec3.x / distance;
	ResultVec.y = vec3.y / distance;
	ResultVec.z = vec3.z / distance;
	return ResultVec;
}

Vec3 Vec3::Cross(Vec3 vecA, Vec3 vecB)
{
	Vec3 newVec = {
		vecA.y * vecB.z - vecA.z * vecB.y,  
		vecA.z * vecB.x - vecA.x * vecB.z,  
		vecA.x * vecB.y - vecA.y * vecB.x
	};
		
	return newVec;
}

float Vec3::MyDot(Vec3 vecA, Vec3 vecB)
{
    float result = (vecA.x * vecB.x) + (vecA.y * vecB.y) + (vecA.z * vecB.z);
	
	return result;
}