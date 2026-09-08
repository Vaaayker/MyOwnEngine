#include "mat4.hpp"

mat4::mat4()
{
    for(int i = 0; i < 4; i++)
    {
        for(int n = 0; n < 4; n++)
        {
            mat[i][n] = 0.0f;
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
                result.mat[i][n] = result.mat[i][n] + 
                    (matrixA.mat[i][k] * matrixB.mat[k][n]);
            }
        }
    }

    return result;
}