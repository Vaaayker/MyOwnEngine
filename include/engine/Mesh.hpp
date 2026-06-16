#pragma once
#include "Vertex.hpp"
#include <vector>
using namespace std;

class Mesh
{
private:
    vector<Vertex> vertices;
    vector<uint32_t> indices;

public:
    void CreateRoom();
    void setRoomIndices();
    void AddVertices(Vec3 position, Vec3 normal);
    void AddTriangle(uint a, uint b, uint c);
};