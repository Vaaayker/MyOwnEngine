#include "MathOperation.hpp"

void MathOperation::MultiplyFourSquareMatrix(float result[4][4], float matrixA[4][4], float matrixB[4][4]) // рядок на стовпець
{
    for(int i = 0; i < 2; i++)
    {
        for(int n = 0; n < 2; n++)
        {
            result[i][n] = 0;

            for(int k = 0; k < 2; k++)
            {
                result[i][n] = result[i][n] + 
                    (matrixA[i][k] * matrixB[k][n]);
            }
        }
    }
}