#include "mat4.hpp"
#include "Vec4.hpp"
#include <cmath>

mat4::mat4()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                mat[i][j] = 1.0f;
            }
            else    
            {
                mat[i][j] = 0.0f;
            }
        }
    }
}

mat4 mat4::MultiplyFourSquareMatrix(const mat4& matrixA, const mat4& matrixB) // row on col
{
    mat4 result{};

    for(int i = 0; i < 4; i++)
    {
        for(int n = 0; n < 4; n++)
        {
            result.mat[i][n] = 0;

            for(int k = 0; k < 4; k++)
            {
                result.mat[i][n] = result.mat[i][n] 
                + (matrixA.mat[i][k] * matrixB.mat[k][n]);
            }
        }
    }

    return result;
}

Vec4 mat4::MultiplyMatrixOnVec(const mat4& matrixA, const Vec4& vec)
{
    Vec4 result{};
    for (std::uint32_t i = 0; i < 4; i++)
    {
        result[i] = matrixA.mat[i][0] * vec[0] +
                    matrixA.mat[i][1] * vec[1] +
                    matrixA.mat[i][2] * vec[2] +
                    matrixA.mat[i][3] * vec[3];
    }
    return result;
}

mat4 mat4::matricesSum(const mat4& first, const mat4& second)
{
    mat4 result{};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.mat[i][j] = first.mat[i][j] + second.mat[i][j];
        }
    }
    return result;
}

mat4 mat4::matricesDiff(const mat4& first, const mat4& second)
{
    mat4 result{};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.mat[i][j] = first.mat[i][j] - second.mat[i][j];
        }
    }
    return result;
}

float& mat4::operator()(std::uint32_t row, std::uint32_t column)
{
    return mat[row][column];
}

Vec4 mat4::GetRow(std::uint32_t row) const
{
    Vec4 result{};
    for (std::uint32_t i = 0; i < 4; i++)
    {
        result[i] = mat[row][i];
    }
    return result;
}

Vec4 mat4::GetColumn(std::uint32_t column) const
{
    Vec4 result{};
    for (std::uint32_t i = 0; i < 4; i++)
    {
        result[i] = mat[i][column];
    }
    return result;
}

void mat4::Translation(const Vec4& translation)
{
    mat[0][3] = translation[0];
    mat[1][3] = translation[1];
    mat[2][3] = translation[2];
}

void mat4::Scale(const Vec4& scale)
{
    mat[0][0] *= scale[0];
    mat[1][1] *= scale[1];
    mat[2][2] *= scale[2];
}


void mat4::LookAt(const Vec4& eye, const Vec4& target, const Vec4& up)
{
    // за векторним добутком(висоти на глибину) отримати вісь ширини
    Vec4 directionView = Vec4::Normalize(target - eye); // напрямок глибини
    Vec4 directionUp = Vec4::Normalize(up -  eye); // напрямок висоти
    Vec4 directionRight = Vec4::Cross(directionView, directionUp); // напрямок ширини

    // Оновлення матриці погляду
    mat[0][0] = directionRight[0];
    mat[0][1] = directionRight[1];
    mat[0][2] = directionRight[2];
    mat[1][0] = directionUp[0];
    mat[1][1] = directionUp[1];
    mat[1][2] = directionUp[2];
    mat[2][0] = directionView[0];
    mat[2][1] = directionView[1];
    mat[2][2] = directionView[2];
    mat[3][3] = 1.0f;
}

void mat4::Perspective(float fov, float aspect, float nearPlane, float farPlane)
{
    constexpr float PI = 3.14159265358979323846f;
    const float fieldOfViewRadians = fov * PI / 180.0f;

    mat[0][0] = 1 / (aspect * std::tan(fieldOfViewRadians / 2));
    mat[1][1] = 1 / std::tan(fieldOfViewRadians / 2);
    mat[2][2] = farPlane / (farPlane - nearPlane);
    mat[2][3] = (nearPlane * farPlane) / (nearPlane - farPlane);
    mat[3][2] = 1.0f; 
}

void mat4::RotationX(float angle)
{
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    mat[1][1] = cosAngle;
    mat[1][2] = -sinAngle;
    mat[2][1] = sinAngle;
    mat[2][2] = cosAngle;
}

void mat4::RotationY(float angle)
{
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    mat[0][0] = cosAngle;
    mat[0][2] = sinAngle;
    mat[2][0] = -sinAngle;
    mat[2][2] = cosAngle;
}

void mat4::RotationZ(float angle)
{
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    mat[0][0] = cosAngle;
    mat[0][1] = -sinAngle;
    mat[1][0] = sinAngle;
    mat[1][1] = cosAngle;
}

mat4 operator*(const mat4& first, const mat4& second)
{
    return mat4::MultiplyFourSquareMatrix(first, second);
}

Vec4 operator*(const mat4& matrix, const Vec4& vec)
{
    return mat4::MultiplyMatrixOnVec(matrix, vec);
}