#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include "FrameResources.hpp"

class VulkanContext; // forward declaration

/**
 * @brief Manages the Vulkan command pool and per-frame command buffers.
 *
 * Creates and owns a Vulkan command pool associated with the graphics queue
 * family and allocates one command buffer for each frame in flight.
 *
 * Destroying the command pool also releases all command buffers allocated
 * from it.
 */


class CommandPool
{
public:
    CommandPool() = default;
    CommandPool(const CommandPool&) = delete;
    CommandPool& operator=(const CommandPool&) = delete;
    ~CommandPool();

    void Create(const VulkanContext& context);
    void Destroy();

    // getter
    vk::CommandBuffer GetCommandBuffer(std::uint32_t frameIndex) const; 

private:
    void createCommandPool(const VulkanContext& context);
    void allocateCommandBuffersAndInitializeSyncObject();

private:
    vk::Device device{}; // Borrowed handle

    vk::CommandPool commandPool{};
    std::vector<vk::CommandBuffer> commandBuffers = std::vector<vk::CommandBuffer>(MAX_FRAMES_IN_FLIGHT);
};