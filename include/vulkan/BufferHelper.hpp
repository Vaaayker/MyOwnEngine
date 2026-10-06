#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

// forward declaration
class Mesh;
class VulkanContext;
struct Vertex;  

/** 
 *  @brief Manages vertex and index buffers, including staging buffers 
 * and their associated Vulkan memory. 
 */

class BufferHelper
{
public:
    BufferHelper() = default;
    void Create(const Mesh& mesh, const VulkanContext& context);
    void Destroy();
    BufferHelper(const BufferHelper&) = delete;
    BufferHelper& operator=(const BufferHelper&) = delete;

    vk::Buffer GetVertexBuffer() const;
    vk::Buffer GetIndicesBuffer() const;
    vk::Buffer GetStageVertexBuffer() const;
    vk::Buffer GetStageIndicesBuffer() const;

private:
// borowed handles
    vk::Device device{};
    vk::PhysicalDevice physicalDevice{};
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};

private:
    void CreateBuffers();
    void DefineMemoryProperties();
    void AllocateMemory();
    std::uint32_t FindMemoryTypeIndex(vk::PhysicalDeviceMemoryProperties propertiesDevice, vk::MemoryRequirements requirements, vk::MemoryPropertyFlags requiredProperties);
    vk::Buffer verticesStageBuffer{}; 
    vk::Buffer indicesStageBuffer{};
    vk::Buffer vertexBuffer{};
    vk::Buffer indicesBuffer{};

    vk::MemoryRequirements requirementsVertexBuffer{};
    vk::MemoryRequirements requirementsStageVertexBuffer{};
    vk::MemoryRequirements requirementsIndicesBuffer{};
    vk::MemoryRequirements requirementsStageIndicesBuffer{};

    vk::PhysicalDeviceMemoryProperties memoryProperties{};

    vk::DeviceSize sizeVertices{};
    vk::DeviceSize sizeIndices{};

    std::uint32_t indexMemoryTypeLocalBuffer{};
    std::uint32_t indexMemoryTypeHostBuffer{}; 
    vk::MemoryPropertyFlags propertiesLocalBuffer{}; 
    vk::MemoryPropertyFlags propertiesHostBuffer{};

    vk::DeviceMemory verticesBufferMemory{};
    vk::DeviceMemory verticesStageBufferMemory{};
    vk::DeviceMemory indicesBufferMemory{};
    vk::DeviceMemory indicesStagesBufferMemory{};
};
