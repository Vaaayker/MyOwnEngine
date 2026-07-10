#include "Swapchain.hpp"
#include "VulkanContext.hpp"
#include <cassert>
#include <algorithm>
#include <limits>


Swapchain::~Swapchain()
{
    Destroy();
}

void Swapchain::Create(const VulkanContext& context, SDL_Window* window)
{
    // Define Borowed Handle   
    surface = context.GetSurface();
    physicalDevice = context.GetPhysicalDevice();
    device = context.GetDevice();

    surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR( surface );
    availableFormats = physicalDevice.getSurfaceFormatsKHR( surface );
    supportedPresentMode = physicalDevice.getSurfacePresentModesKHR( surface );

    swapChainSurfaceFormat = CorrectFormat(availableFormats);
    swapChainExtent = chooseSwapExtent(surfaceCapabilities, window);
    minImageCount = chooseSwapMinImageCount(surfaceCapabilities);

    vk::SwapchainCreateInfoKHR  swapChainCreateInfo{};

    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.minImageCount = minImageCount;
    swapChainCreateInfo.imageFormat = swapChainSurfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = swapChainSurfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = swapChainExtent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
    swapChainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
    swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    swapChainCreateInfo.presentMode = chooseSwapPresentMode(supportedPresentMode);
    swapChainCreateInfo.clipped = true;
    
    swapChain = device.createSwapchainKHR(swapChainCreateInfo);
    swapChainImages = device.getSwapchainImagesKHR(swapChain);
}

void Swapchain::Destroy()
{
    if(swapChain)
    {
        device.destroySwapchainKHR(swapChain);
        swapChain = nullptr;
    }
}

vk::SurfaceFormatKHR Swapchain::CorrectFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    assert(!availableFormats.empty());

   for(size_t i = 0; i < availableFormats.size(); i++)
   {
        if(availableFormats[i].format == vk::Format::eB8G8R8A8Srgb && availableFormats[i].colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return availableFormats[i];
        }
   } 

   return availableFormats[0];
}


vk::PresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for(size_t i = 0; i < availablePresentModes.size(); i++)
    {
        if(availablePresentModes[i] == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentModes[i];
        }
    }

    return vk::PresentModeKHR::eFifo;
}


vk::Extent2D Swapchain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, SDL_Window* window)
{
    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }

    int width = 0, height = 0;
    SDL_GetWindowSizeInPixels(window, &width, &height);

    return {
        std::clamp<uint32_t>(static_cast<uint32_t>(width), capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
        std::clamp<uint32_t>(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height),
    };
}


uint32_t Swapchain::chooseSwapMinImageCount(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities)
{
    uint32_t minImageCount = std::max(3u, surfaceCapabilities.minImageCount);
    if((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount))
    {
        minImageCount = surfaceCapabilities.maxImageCount;
    }
    return minImageCount;
}

