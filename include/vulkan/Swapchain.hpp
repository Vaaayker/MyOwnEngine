#pragma once
#include <vector>
#include <cstdint>
#include "vulkan/vulkan.hpp"
#include "SDL3/SDL.h"

class VulkanContext; // forward declaration

/**
 * @brief Manages the Vulkan swapchain and its image views.
 *
 * Selects the surface format, presentation mode, image count, and image size.
 * Creates and owns the swapchain and image views used for rendering.
 */

class Swapchain
{
public:
    Swapchain() = default;
    Swapchain(const Swapchain&) = delete;
    Swapchain& operator=(const Swapchain&) = delete;
    ~Swapchain();

    void Create(const VulkanContext& context, SDL_Window* window);
    void Destroy();

    // getter
    vk::Format getFormat() const;
    vk::Extent2D getExtent() const;
    const std::vector<vk::ImageView>& GetImageViews() const;
    vk::SwapchainKHR GetSwapchain() const;

private:
    vk::SurfaceFormatKHR CorrectFormat();
    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, SDL_Window* window);
    uint32_t chooseSwapMinImageCount();
    void CreateImageViews();

private:
    // Borowed handle. Owned by VulkanContext class:
    vk::SurfaceKHR surface{};
    vk::PhysicalDevice physicalDevice{};
    vk::Device device{};

    // Owned by this class:
    vk::SurfaceCapabilitiesKHR surfaceCapabilities{};
    std::vector<vk::SurfaceFormatKHR> availableFormats{};
    std::vector<vk::PresentModeKHR> supportedPresentMode{};
    vk::SwapchainKHR swapChain{};
    std::vector<vk::Image> swapChainImages{};
    std::vector<vk::ImageView> swapChainImageViews{};
    vk::SurfaceFormatKHR swapChainSurfaceFormat{};
    vk::Extent2D swapChainExtent{};
    uint32_t minImageCount;
};
