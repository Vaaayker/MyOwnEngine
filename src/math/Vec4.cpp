#include "Vec4.hpp"
#include "Vec3.hpp"
#include <cmath>
#include <stdexcept>

Vec4::Vec4()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
}

Vec4::Vec4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vec4 Vec4::makeDirectionVec4(Vec3 vec3)
{
    return {vec3.x, vec3.y, vec3.z, 0};
}

Vec4 Vec4::makePointVec4(Vec3 vec3)
{
    return {vec3.x, vec3.y, vec3.z, 1};
}

Vec4 Vec4::Normalize(Vec4 vec4)
{
	Vec4 ResultVec{};
	float distance = std::sqrt( vec4.x * vec4.x + vec4.y * vec4.y + vec4.z * vec4.z + vec4.w * vec4.w);
	if(distance == 0.0f)
	{
		return ResultVec; 
	}
	ResultVec.x = vec4.x / distance;
	ResultVec.y = vec4.y / distance;
	ResultVec.z = vec4.z / distance;
	ResultVec.w = vec4.w / distance;
	return ResultVec;
}

Vec4 Vec4::Cross(Vec4 vecA, Vec4 vecB)
{
	Vec4 newVec = {
		vecA.y * vecB.z - vecA.z * vecB.y,  
		vecA.z * vecB.x - vecA.x * vecB.z,  
		vecA.x * vecB.y - vecA.y * vecB.x,
		0.0f
	};

	return newVec;
}

float Vec4::MyDot(Vec4 vecA, Vec4 vecB)
{
    float result = (vecA.x * vecB.x) + (vecA.y * vecB.y) + (vecA.z * vecB.z) + (vecA.w * vecB.w);

	return result;
}

float& Vec4::operator[](std::uint32_t index)
{
	switch (index)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw std::out_of_range("Index out of range for Vec4");
	}
}

const float& Vec4::operator[](std::uint32_t index) const
{
	switch (index)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw std::out_of_range("Index out of range for Vec4");
	}
}

Vec4 Vec4::operator+(const Vec4& other) const
{
    return Vec4(
        x + other.x,
        y + other.y,
        z + other.z,
        w + other.w
    );
}

Vec4 Vec4::operator-(const Vec4& other) const
{
    return Vec4(
        x - other.x,
        y - other.y,
        z - other.z,
        w - other.w
    );
}