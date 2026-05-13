#include "Room.hpp"

Room::Room()
{
    width = 10.0f;
    height = 5.0f;
    depth = 7.0f;
    SetMatrix();
    AddElementMatrix();
}

void Room::AddElementMatrix()
{
    modelMatrix[0][0] = width;
    modelMatrix[1][1] = height;
    modelMatrix[2][2] = depth;
}

void Room::SetMatrix()
{
    for(int rows = 0; rows < 4; rows++)
    {
        for(int cols = 0; cols < 4; cols++)
        {
            if(rows == cols)
            {
                modelMatrix[rows][cols] = 1.0f;
            }
            else
            {
                modelMatrix[rows][cols] = 0.0f;
            }
        }
    }
}


