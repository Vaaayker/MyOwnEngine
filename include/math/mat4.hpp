#pragma once 
#include <cstdint>

// forward declaration
struct Vec4;    

/**
 * @brief 4x4 transformation matrix using column-vector convention.
 *
 * - Vectors are columns: v' = M * v.
 * - Basis vectors are stored in columns.
 * - Translation is stored in column 3.
 * - Transform composition: M = T * R * S.
 * - Element access: M(row, column).
 * - Matrix storage is column-major.
 */

struct mat4
{
    float mat[4][4];

    mat4();

    static mat4 MultiplyFourSquareMatrix(const mat4& matrixA, const mat4& matrixB); // row on col
    static Vec4 MultiplyMatrixOnVec(const mat4& matrixA, const Vec4& vec);

    static mat4 matricesSum (const mat4& first, const mat4& second);
    static mat4 matricesDiff (const mat4& first, const mat4& second);

    float& operator()(std::uint32_t row, std::uint32_t column);
    
    Vec4 GetRow(std::uint32_t row) const;
    Vec4 GetColumn(std::uint32_t column) const;

    void Translation(const Vec4& translation);
    void Scale(const Vec4& scale);
    void LookAt(const Vec4& eye, const Vec4& target, const Vec4& up);
    void Perspective(float fov, float aspect, float nearPlane, float farPlane);

    void RotationX(float angle);
    void RotationY(float angle);
    void RotationZ(float angle);
};

mat4 operator*(const mat4& first, const mat4& second);
Vec4 operator*(const mat4& matrix, const Vec4& vec);

