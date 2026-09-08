#include "Room.hpp"

Room::Room()
{
    width = 10.0f;
    height = 5.0f;
    depth = 7.0f;

    // Add Element to Model Matrix
    modelMatrix.mat[0][0] = width;
    modelMatrix.mat[1][1] = height;
    modelMatrix.mat[2][2] = depth;
}





