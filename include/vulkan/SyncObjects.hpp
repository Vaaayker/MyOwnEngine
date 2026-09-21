#pragma once
#include <vulkan/vulkan.hpp>
#include "FrameResources.hpp"
#include <cstddef>
#include <vector>

class VulkanContext; // forward declarations
class Swapchain; // forward declarations


/**
 * @brief Manages synchronization objects for frames processed in parallel.
 *
 * Creates, stores, and destroys semaphores and fences for each frame
 * according to the MAX_FRAMES_IN_FLIGHT value.
 */

class SyncObjects
{
public:
    SyncObjects() = default;
    SyncObjects(const SyncObjects&) = delete;
    SyncObjects& operator=(const SyncObjects&) = delete;
    ~SyncObjects();

    void Create(const VulkanContext& context, const Swapchain& swapchain);
    void ReinitializeResources(const Swapchain& swapchain);
    void DestroyResources();
    void Destroy();

    // getters
    vk::Semaphore getImageAvailableSemaphore(std::uint32_t frameIndex) const;
    const std::vector<vk::Semaphore>& getRenderFinishedSemaphore() const;
    vk::Fence getInFlightFence(std::uint32_t frameIndex) const;

private:
    void createSemaphores();
    void createFences();

private:
    vk::Device device{}; // borrowed handle
    std::size_t renderFinishedCount{};

    std::vector<vk::Semaphore> imageAvailableSemaphores = std::vector<vk::Semaphore>(MAX_FRAMES_IN_FLIGHT);
    std::vector<vk::Fence> inFlightFences = std::vector<vk::Fence>(MAX_FRAMES_IN_FLIGHT);
    std::vector<vk::Semaphore> renderFinishedSemaphores;

};