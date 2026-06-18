#pragma once
#include "vector"
#include <vulkan/vulkan.h>
using namespace

class Swapchain
{
private:
    VkSwapchainKHR swapchain;
    vector<VkImage> images;
    vector<VkImageView> imageViews;
    VkFormat imageFormat;
    VkExtent2D extent;
public:
    SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const vkGetPhysicalDeviceSurfaceCapabilitesKHR& capabilities, SDL_Window* window);
    void CreateSwapchain(SDL_Window* window);
    void CreateImageViews();
};
