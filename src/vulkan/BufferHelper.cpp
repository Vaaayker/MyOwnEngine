#include "BufferHelper.hpp"
#include "VulkanContext.hpp"
#include "Mesh.hpp"

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

    void* dataVerices = device.mapMemory(verticesStageBufferMemory, 0, sizeVertices);
    std::memcpy(dataVerices, vertices.data(), sizeVertices);
    device.unmapMemory(verticesStageBufferMemory);

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

    void* dataIndices = device.mapMemory(indicesStagesBufferMemory, 0, sizeIndices);
    std::memcpy(dataIndices, indices.data(), sizeIndices);
    device.unmapMemory(indicesStagesBufferMemory);
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
