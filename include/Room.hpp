#pragma once

class Room
{
private:
    float width;
    float height;
    float depth;
    float modelMatrix[4][4];

public:
    void AddElementMatrix();
    void SetMatrix();
    Room();

};