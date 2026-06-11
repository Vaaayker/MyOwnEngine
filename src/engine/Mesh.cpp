#include "Mesh.hpp"
#include "Vec3.hpp"

void Mesh::CreateRoom()
{
    // перша стіна
    Vec3 NormalFirstWall = {1, 0, 0};
    vertices.push_back({{0, 0, 0}, NormalFirstWall}); // 0
    vertices.push_back({{0, 0, 7}, NormalFirstWall}); // 1
    vertices.push_back({{0, 5, 0}, NormalFirstWall}); // 2
    vertices.push_back({{0, 5, 7}, NormalFirstWall}); // 3
    // indices: 0, 2, 1,
    //          1, 2, 3

    // друга стіна
    Vec3 NormalSecondWall = {0, 0, 1};
    vertices.push_back({{0, 0, 0}, NormalSecondWall}); // 4
    vertices.push_back({{10, 0, 0}, NormalSecondWall}); // 5
    vertices.push_back({{0, 5, 0}, NormalSecondWall}); // 6
    vertices.push_back({{10, 5, 0}, NormalSecondWall}); // 7
    // indices: 4, 6, 5,
    //          5, 6, 7

    // третя стіна
    Vec3 NormalThirdWall = {-1, 0, 0};
    vertices.push_back({{10, 0, 0}, NormalThirdWall}); // 8
    vertices.push_back({{10, 0, 7}, NormalThirdWall}); // 9
    vertices.push_back({{10, 5, 0}, NormalThirdWall}); // 10
    vertices.push_back({{10, 5, 7}, NormalThirdWall}); // 11
    // indices: 8, 10, 9,
    //          9, 10, 11

    // четверта стіна
    Vec3 NormalFourthWall = {0, 0, -1};
    vertices.push_back({{10, 0, 7}, NormalFourthWall}); // 12
    vertices.push_back({{0, 0, 7}, NormalFourthWall}); // 13
    vertices.push_back({{10, 5, 7}, NormalFourthWall}); // 14
    vertices.push_back({{0, 5, 7}, NormalFourthWall}); // 15
    // indices: 12, 14, 13,
    //          13, 14, 15 


    // п'ята стіна
    Vec3 NormalFifthWall = {0, -1, 0};
    vertices.push_back({{0, 5, 0}, NormalFifthWall}); // 16
    vertices.push_back({{10, 5, 0}, NormalFifthWall}); // 17
    vertices.push_back({{0, 5, 7}, NormalFifthWall}); // 18
    vertices.push_back({{10, 5, 7}, NormalFifthWall});  // 19
    // indices: 16, 18, 17,
    //          17, 18, 19

    // шоста стіна
    Vec3 NormalSixthWall = {0, 1, 0};
    vertices.push_back({{0, 0, 0}, NormalSixthWall}); // 20
    vertices.push_back({{10, 0, 0}, NormalSixthWall}); // 21
    vertices.push_back({{0, 0, 7}, NormalSixthWall}); // 22
    vertices.push_back({{10, 0, 7}, NormalSixthWall});  // 23
    // indices: 20, 22, 21,
    //          21, 22, 23
}

void Mesh::setRoomIndices()
{
    uint indicesSec[36] = {0, 2, 1, 1, 2, 3, 4, 6, 5, 5, 6, 7, 8, 10, 9, 9, 10, 11, 12, 14, 13, 13, 14, 15, 16, 18, 17, 17, 18, 19, 20, 22, 21, 21, 22, 23};
    for(int i = 0; i < 36; i++)
    {
        indices.push_back(indicesSec[i]);
    }
}

void Mesh::AddVertices(Vec3 position, Vec3 normal)
{
    vertices.push_back({position, normal});  
}

void Mesh::AddTriangle(uint a, uint b, uint c)
{
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
}


