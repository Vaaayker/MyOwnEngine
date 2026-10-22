#include "BufferHelper.hpp"
#include "VulkanContext.hpp"
#include "Mesh.hpp"
#include "FrameResources.hpp"

#include <cstring>
#include <stdexcept>

void BufferHelper::Create(const Mesh& mesh, const VulkanContext& context)
{
    device = context.GetDevice();
    physicalDevice = context.GetPhysicalDevice();

    vertices = mesh.GetVertices();
    indices = mesh.GetIndices();

    sizeVertices = static_cast<vk::DeviceSize>(vertices.size() * sizeof(vertices[0]));
    sizeIndices = static_cast<vk::DeviceSize>(indices.size() * sizeof(indices[0]));

    CreateBuffers();
    DefineMemoryProperties();
    AllocateMemory();
}

// destroy 
void BufferHelper::Destroy()
{
    DestroyUniformBuffers();

    // unmap persistent staging memory
    if (verticesStageMapped != nullptr)
    {
        device.unmapMemory(verticesStageBufferMemory);
        verticesStageMapped = nullptr;
    }

    if (indicesStageMapped != nullptr)
    {
        device.unmapMemory(indicesStagesBufferMemory);
        indicesStageMapped = nullptr;
    }

    // destroy buffers
    device.destroyBuffer(indicesStageBuffer);
    device.freeMemory(indicesStagesBufferMemory);

    device.destroyBuffer(indicesBuffer);
    device.freeMemory(indicesBufferMemory);

    device.destroyBuffer(verticesStageBuffer);
    device.freeMemory(verticesStageBufferMemory);

    device.destroyBuffer(vertexBuffer);
    device.freeMemory(verticesBufferMemory);

    // get the null to borowed handles
    vertices.clear();
    indices.clear();
    device = nullptr;
    physicalDevice = nullptr;
}

void BufferHelper::CreateBuffers()
{
    vk::BufferCreateInfo infoVertices{};
    infoVertices.size = sizeVertices;
    infoVertices.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst; // нашо побітовка
    infoVertices.sharingMode = vk::SharingMode::eExclusive;
    // infoVertices.queueFamilyIndexCount - при eExclusive вказувати не потрібно
    // eExclusive - buffer використовується однією queue family — graphics queue family.
    vertexBuffer = device.createBuffer(infoVertices);
    requirementsVertexBuffer = device.getBufferMemoryRequirements(vertexBuffer);
    infoVertices.usage = vk::BufferUsageFlagBits::eTransferSrc;
    verticesStageBuffer = device.createBuffer(infoVertices);
    requirementsStageVertexBuffer = device.getBufferMemoryRequirements(verticesStageBuffer);

    vk::BufferCreateInfo infoIndices{};
    infoIndices.size = sizeIndices;
    infoIndices.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst;
    infoIndices.sharingMode = vk::SharingMode::eExclusive;
    indicesBuffer = device.createBuffer(infoIndices);
    requirementsIndicesBuffer = device.getBufferMemoryRequirements(indicesBuffer);
    infoIndices.usage = vk::BufferUsageFlagBits::eTransferSrc;
    indicesStageBuffer = device.createBuffer(infoIndices);
    requirementsStageIndicesBuffer = device.getBufferMemoryRequirements(indicesStageBuffer);
}

void BufferHelper::AllocateMemory()
{
    // чотири для кожного буфера
    vk::MemoryAllocateInfo vertexBufferInfo{}; 
    vertexBufferInfo.allocationSize = sizeVertices;
    vertexBufferInfo.memoryTypeIndex = indexMemoryTypeLocalBuffer;
    verticesBufferMemory = device.allocateMemory(vertexBufferInfo);
    device.bindBufferMemory(vertexBuffer, verticesBufferMemory, 0);

    vk::MemoryAllocateInfo vertexStagesBufferInfo{}; 
    vertexStagesBufferInfo.allocationSize = sizeVertices;
    vertexStagesBufferInfo.memoryTypeIndex = indexMemoryTypeHostBuffer;
    verticesStageBufferMemory = device.allocateMemory(vertexStagesBufferInfo);
    device.bindBufferMemory(verticesStageBuffer, verticesStageBufferMemory, 0);

    // persistent mapping - unmap only in Destroy()
    verticesStageMapped = device.mapMemory(verticesStageBufferMemory, 0, sizeVertices);
    std::memcpy(verticesStageMapped, vertices.data(), sizeVertices);

    vk::MemoryAllocateInfo indicesBufferInfo{}; 
    indicesBufferInfo.allocationSize = sizeIndices;
    indicesBufferInfo.memoryTypeIndex = indexMemoryTypeLocalBuffer;
    indicesBufferMemory = device.allocateMemory(indicesBufferInfo);
    device.bindBufferMemory(indicesBuffer, indicesBufferMemory, 0);

    vk::MemoryAllocateInfo indicesStageBufferInfo{}; 
    indicesStageBufferInfo.allocationSize = sizeIndices;
    indicesStageBufferInfo.memoryTypeIndex = indexMemoryTypeHostBuffer;
    indicesStagesBufferMemory = device.allocateMemory(indicesStageBufferInfo);
    device.bindBufferMemory(indicesStageBuffer, indicesStagesBufferMemory, 0);

    // persistent mapping - unmap only in Destroy()
    indicesStageMapped = device.mapMemory(indicesStagesBufferMemory, 0, sizeIndices);
    std::memcpy(indicesStageMapped, indices.data(), sizeIndices);
}

void BufferHelper::DefineMemoryProperties()
{
    memoryProperties = physicalDevice.getMemoryProperties();

    propertiesLocalBuffer = vk::MemoryPropertyFlagBits::eDeviceLocal;
    indexMemoryTypeLocalBuffer = FindMemoryTypeIndex(memoryProperties, requirementsVertexBuffer, propertiesLocalBuffer);

    propertiesHostBuffer = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;   
    indexMemoryTypeHostBuffer = FindMemoryTypeIndex(memoryProperties, requirementsStageVertexBuffer, propertiesHostBuffer);
}

std::uint32_t BufferHelper::FindMemoryTypeIndex(vk::PhysicalDeviceMemoryProperties propertiesDevice, vk::MemoryRequirements requirements, vk::MemoryPropertyFlags requiredProperties)
{
    for (std::uint32_t i = 0; i < propertiesDevice.memoryTypeCount; i++)
    {
        if ((requirements.memoryTypeBits & (1 << i)) && (propertiesDevice.memoryTypes[i].propertyFlags & requiredProperties) == requiredProperties)
        {
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type");
}

vk::Buffer BufferHelper::GetVertexBuffer() const
{
    return vertexBuffer;
}

vk::Buffer BufferHelper::GetIndicesBuffer() const
{
    return indicesBuffer;
}

vk::Buffer BufferHelper::GetStageVertexBuffer() const
{
    return verticesStageBuffer;
}

vk::Buffer BufferHelper::GetStageIndicesBuffer() const
{
    return indicesStageBuffer;
}

void BufferHelper::CreateUniformBuffers(vk::DeviceSize bufferSize)
{
    uniformBufferSize = bufferSize;

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT, nullptr);

    memoryProperties = physicalDevice.getMemoryProperties();

    vk::MemoryPropertyFlags properties =
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent;

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        // Create uniform buffer
        vk::BufferCreateInfo bufferInfo{};

        bufferInfo.size = bufferSize;
        bufferInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        uniformBuffers[i] = device.createBuffer(bufferInfo);

        // Get memory requirements
        vk::MemoryRequirements requirements =
            device.getBufferMemoryRequirements(uniformBuffers[i]);

        // Find suitable memory type
        uint32_t memoryTypeIndex =
            FindMemoryTypeIndex(memoryProperties, requirements, properties);

        // Allocate memory
        vk::MemoryAllocateInfo memoryInfo{};

        memoryInfo.allocationSize = requirements.size;
        memoryInfo.memoryTypeIndex = memoryTypeIndex;

        uniformBuffersMemory[i] = device.allocateMemory(memoryInfo);

        // Bind memory to buffer
        device.bindBufferMemory(
            uniformBuffers[i],
            uniformBuffersMemory[i],
            0
        );

        // Persistent mapping
        uniformBuffersMapped[i] = device.mapMemory(
            uniformBuffersMemory[i],
            0,
            bufferSize
        );
    }
}

void BufferHelper::UpdateUniformBuffer(uint32_t currentFrame, const void* data, vk::DeviceSize dataSize)
{
    if (currentFrame >= uniformBuffersMapped.size())
    {
        throw std::out_of_range("Invalid uniform buffer frame index");
    }

    if (data == nullptr || dataSize > uniformBufferSize)
    {
        throw std::runtime_error("Invalid uniform buffer update data");
    }

    std::memcpy(
        uniformBuffersMapped[currentFrame],
        data,
        static_cast<size_t>(dataSize)
    );
}

vk::Buffer BufferHelper::GetUniformBuffer(uint32_t frame) const
{
    return uniformBuffers.at(frame);
}

vk::DeviceSize BufferHelper::GetUniformBufferSize() const
{
    return uniformBufferSize;
}

void BufferHelper::DestroyUniformBuffers()
{
    for (uint32_t i = 0; i < uniformBuffers.size(); i++)
    {
        // Unmap memory
        if (uniformBuffersMapped[i] != nullptr)
        {
            device.unmapMemory(uniformBuffersMemory[i]);
            uniformBuffersMapped[i] = nullptr;
        }

        // Destroy buffer
        if (uniformBuffers[i])
        {
            device.destroyBuffer(uniformBuffers[i]);
            uniformBuffers[i] = nullptr;
        }

        // Free memory
        if (uniformBuffersMemory[i])
        {
            device.freeMemory(uniformBuffersMemory[i]);
            uniformBuffersMemory[i] = nullptr;
        }
    }

    uniformBuffers.clear();
    uniformBuffersMemory.clear();
    uniformBuffersMapped.clear();

    uniformBufferSize = 0;
}
