#include "Mesh.hpp"

void Mesh::CreateTriangle()
{
    vertices.clear();
    indices.clear();

    vertices.reserve(3);
    indices.reserve(3);

    Vec3 Normal = {0.0f, 0.0f, 0.0f};
    vertices.push_back({{0.0f, -0.5f, 0.0f}, Normal, {1.0f, 0.0f, 0.0f}}); // 0
    vertices.push_back({{0.5f, 0.5f, 0.0f}, Normal, {0.0f, 1.0f, 0.0f}}); // 1
    vertices.push_back({{-0.5f, 0.5f, 0.0f}, Normal, {0.0f, 0.0f, 1.0f}}); // 2

    for(std::uint32_t i = 0; i < 3; i++)
    {
        indices.push_back(i);
    }
}

void Mesh::CreateRoom()
{
    vertices.clear();
    indices.clear();

    vertices.reserve(24);
    indices.reserve(36);

    // first wall
    Vec3 NormalFirstWall = {1.0f, 0.0f, 0.0f};
    vertices.push_back({{0.0f, 0.0f, 0.0f}, NormalFirstWall}); // 0
    vertices.push_back({{0.0f, 0.0f, 7.0f}, NormalFirstWall}); // 1
    vertices.push_back({{0.0f, 5.0f, 0.0f}, NormalFirstWall}); // 2
    vertices.push_back({{0.0f, 5.0f, 7.0f}, NormalFirstWall}); // 3
    // indices: 0, 2, 1,
    //          1, 2, 3

    // second wall
    Vec3 NormalSecondWall = {0.0f, 0.0f, 1.0f};
    vertices.push_back({{0.0f, 0.0f, 0.0f}, NormalSecondWall}); // 4
    vertices.push_back({{10.0f, 0.0f, 0.0f}, NormalSecondWall}); // 5
    vertices.push_back({{0.0f, 5.0f, 0.0f}, NormalSecondWall}); // 6
    vertices.push_back({{10.0f, 5.0f, 0.0f}, NormalSecondWall}); // 7
    // indices: 4, 6, 5,
    //          5, 6, 7

    // third wall
    Vec3 NormalThirdWall = {-1.0f, 0.0f, 0.0f};
    vertices.push_back({{10.0f, 0.0f, 0.0f}, NormalThirdWall}); // 8
    vertices.push_back({{10.0f, 0.0f, 7.0f}, NormalThirdWall}); // 9
    vertices.push_back({{10.0f, 5.0f, 0.0f}, NormalThirdWall}); // 10
    vertices.push_back({{10.0f, 5.0f, 7.0f}, NormalThirdWall}); // 11
    // indices: 8, 10, 9,
    //          9, 10, 11

    // fourth wall
    Vec3 NormalFourthWall = {0.0f, 0.0f, -1.0f};
    vertices.push_back({{10.0f, 0.0f, 7.0f}, NormalFourthWall}); // 12
    vertices.push_back({{0.0f, 0.0f, 7.0f}, NormalFourthWall}); // 13
    vertices.push_back({{10.0f, 5.0f, 7.0f}, NormalFourthWall}); // 14
    vertices.push_back({{0.0f, 5.0f, 7.0f}, NormalFourthWall}); // 15
    // indices: 12, 14, 13,
    //          13, 14, 15 

    // fifth wall
    Vec3 NormalFifthWall = {0.0f, -1.0f, 0.0f};
    vertices.push_back({{0.0f, 5.0f, 0.0f}, NormalFifthWall}); // 16
    vertices.push_back({{10.0f, 5.0f, 0.0f}, NormalFifthWall}); // 17
    vertices.push_back({{0.0f, 5.0f, 7.0f}, NormalFifthWall}); // 18
    vertices.push_back({{10.0f, 5.0f, 7.0f}, NormalFifthWall});  // 19
    // indices: 16, 18, 17,
    //          17, 18, 19

    // sixth wall
    Vec3 NormalSixthWall = {0.0f, 1.0f, 0.0f};
    vertices.push_back({{0.0f, 0.0f, 0.0f}, NormalSixthWall}); // 20
    vertices.push_back({{10.0f, 0.0f, 0.0f}, NormalSixthWall}); // 21
    vertices.push_back({{0.0f, 0.0f, 7.0f}, NormalSixthWall}); // 22
    vertices.push_back({{10.0f, 0.0f, 7.0f}, NormalSixthWall});  // 23
    // indices: 20, 22, 21,
    //          21, 22, 23

    std::uint32_t indicesSec[36] = {0, 2, 1, 1, 2, 3, 4, 6, 5, 5, 6, 7, 8, 10, 9, 9, 10, 11, 12, 14, 13, 13, 14, 15, 16, 18, 17, 17, 18, 19, 20, 22, 21, 21, 22, 23};
    for(int i = 0; i < 36; i++)
    {
        indices.push_back(indicesSec[i]);
    }
}

void Mesh::AddVertex(Vec3 position, Vec3 normal)
{
    vertices.push_back({position, normal});  
}

void Mesh::AddIndex(std::uint32_t num)
{
    indices.push_back(num);
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return vertices;;
}

const std::vector<std::uint32_t>& Mesh::GetIndices() const
{
    return indices;
}
