#pragma once
#include "Vertex.hpp"
#include <vector>
#include <cstdint>
#include "Vec3.hpp"

/**
 * @brief Stores the vertices and indices of a mesh.
 *
 * Provides functions for creating room geometry, adding vertices and
 * triangles, and accessing the stored mesh data.
 */

class Mesh
{
public:
    Mesh() = default;

    void CreateRoom();
    void CreateTriangle();
    void AddVertex(Vec3 position, Vec3 normal);
    void AddIndex(std::uint32_t num);

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<std::uint32_t>& GetIndices() const;

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};