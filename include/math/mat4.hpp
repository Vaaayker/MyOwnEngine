#pragma once 

/**
 * @brief Represents a 4x4 matrix and provides basic matrix operations.
 */

struct mat4
{
    float mat[4][4];

    mat4();
    mat4 MultiplyFourSquareMatrix(const mat4& matrixA, const mat4& matrixB); // row on col
};