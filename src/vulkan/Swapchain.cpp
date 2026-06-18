#include "Swapchain.hpp"
#include <algorithm>

SwapchainSupportDetails Swapchain::QuerySwapchainSupport(VkPhysicalDevice device)
{
    SwapchainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceCapabilitesKHR(device, surface, &formatCount, details.formats.data());

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());

    return details;
}

VkSurfaceFormatKHR Swapchain::ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats)
{
    for()
    {
        if(availableFormats.format == VK_FORMAT_B8G8R8A8_SRGB &&
           availableFormats.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormats;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR Swapchain::ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes)
{
    for()
    {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }
}

// extent - розмір swapchain images
VkExtent2D Swapchain::ChooseSwapExtent(const vkGetPhysicalDeviceSurfaceCapabilitesKHR& capabilities, SDL_Window* window)
{
    int width;
    int height;

    SDL_GetWindowSizeInPizels(window, &width, &height);

    VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtent.width = clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);

    actualExtent.height = clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

void Swapchain::CreateSwapchain(SDL_Window* window)
{
    // SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(physicalDevice);

    // VkSurfaceFormatKHR surfaceFormat = ChooseSwapPresentMode(swapchainSupport.presentModes);

    // VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapchainSupport.presentModes);

    // VkExtent2D extent = ChooseSwapExtent(swapchainSupport.capabilites, window);

    // uint32_t imageCount = swapchainSupport.capabilites.minImageCount + 1;

    // if(swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilites.maxImageCount);
    // {
            //imageCount = swapchainSupport.capabilities.maxImageCount;
    //  }

    VkSwapchainCreateInfoKHR createInfo{};

    createInfo.sType = ;
    createInfo.surface = ;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {
        graphicsFamilyIndex,
        presentFamilyIndex
    };

    if(graphicsFamilyIndex != presentFamilyIndex)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamiluIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamiluIndices = nullptr;
    }

    createInfo.preTransform = swapchainSupport.capabilites.curentTransform;

    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // витягнути images
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

    swapchainImage.resize(imageCount);

    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;
}

void Swapchain::CreateImageViews()
{
    swapchainImageViews.resize(swapchainImages.size());

    for(size_t i = 0; i < swapchainImages.size()l i++)
    {
        VkImageViewCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapchainImages[i];

        createInfo.format = swapchainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;   
     
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;

        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        
    }
}



