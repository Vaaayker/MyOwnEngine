#include <vector>
#include "vulkan/vulkan.hpp"

// Stores information about swapchain support provided by a physical device.
struct SwapchainSupportDetails
{
    vk::SurfaceCapabilitiesKHR capabilities{}; // Image count limits, image size limits, and current surface extent
    std::vector<vk::SurfaceFormatKHR> formats{}; // format of the color, forexample VK_FORMAT_B8G8R8A8_SRGB
    std::vector<vk::PresentModeKHR> presentModes{}; // FIFO, MAILBOX, IMMEDIATE
};