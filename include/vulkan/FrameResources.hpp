#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>

/**
 * @brief Stores the Vulkan resources used by one frame in flight.
 *
 * Contains borrowed handles to the command buffer, semaphores, and fence
 * associated with a single frame.
 */

inline constexpr std::uint32_t MAX_FRAMES_IN_FLIGHT = 2;

struct FrameResources
{
    // borowed handles:
    vk::CommandBuffer CommandBuffer{};
    vk::Semaphore ImageAvailableSemaphore{};
    vk::Fence InFlightFence{};
};
