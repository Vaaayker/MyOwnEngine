#include "mat4.hpp"

mat4::mat4()
{
    for(int i = 0; i < 4; i++)
    {
        for(int n = 0; n < 4; n++)
        {
            mat[i][n] = 0;
        }
    }
}