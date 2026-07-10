#pragma once

#include <vector>
#include <cstdint>

#include "vulkan/vulkan.hpp"
#include "SDL3/SDL.h"

class VulkanContext; // forward declaration

class Swapchain
{
public:
    Swapchain() = default;
    ~Swapchain();

    void Create(const VulkanContext& context, SDL_Window* window);
    void Destroy();

    vk::SurfaceFormatKHR CorrectFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, SDL_Window* window);

    uint32_t chooseSwapMinImageCount(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities);
    

private:
    // Borowed handle. Owned be VulkanContext class.
    vk::SurfaceKHR surface{};
    vk::PhysicalDevice physicalDevice{};
    vk::Device device{};

    // Handle owned by this class:

    vk::SurfaceCapabilitiesKHR surfaceCapabilities{};
    std::vector<vk::SurfaceFormatKHR> availableFormats;
    std::vector<vk::PresentModeKHR> supportedPresentMode;

    vk::SwapchainKHR swapChain{};
    std::vector<vk::Image> swapChainImages;

    vk::SurfaceFormatKHR swapChainSurfaceFormat{};
    vk::Extent2D swapChainExtent{};
    uint32_t minImageCount = 0;
};
