#include "DepthResources.hpp"
#include "VulkanContext.hpp"
#include "Swapchain.hpp"
#include "FrameResources.hpp"
#include <iostream>

void DepthResources::CreateDepthResources(const VulkanContext& context, const Swapchain& swapchain) 
{
    device = context.GetDevice();
    physicalDevice = context.GetPhysicalDevice();

    size_t swapchainImagesCount = swapchain.GetImageViews().size();

    depthImages.resize(swapchainImagesCount);
    depthImageMemories.resize(swapchainImagesCount);
    depthImageViews.resize(swapchainImagesCount);

    for (size_t i = 0; i < swapchainImagesCount; ++i) 
    {
        vk::Extent2D swapchainExtent = swapchain.getExtent(); 
        
        vk::ImageCreateInfo imageInfo{};
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = swapchainExtent.width;
        imageInfo.extent.height = swapchainExtent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = vk::Format::eD32Sfloat; 
        imageInfo.tiling = vk::ImageTiling::eOptimal;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;

        depthImages[i] = device.createImage(imageInfo);
    }

    AllocateDepthImagesMemory();

    CreateDepthImageViews();
}

std::uint32_t DepthResources::FindMemoryTypeIndex(vk::PhysicalDeviceMemoryProperties propertiesDevice, vk::MemoryRequirements requirements, vk::MemoryPropertyFlags requiredProperties)
{
    for (std::uint32_t i = 0; i < propertiesDevice.memoryTypeCount; i++)
    {
        if ((requirements.memoryTypeBits & (1 << i)) && (propertiesDevice.memoryTypes[i].propertyFlags & requiredProperties) == requiredProperties)
        {
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type for depth resources");
}

void DepthResources::AllocateDepthImagesMemory() 
{
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    for (size_t i = 0; i < depthImages.size(); ++i) 
    {
        vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements(depthImages[i]);

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryTypeIndex(
            memProperties, 
            memRequirements, 
            vk::MemoryPropertyFlagBits::eDeviceLocal
        );

        depthImageMemories[i] = device.allocateMemory(allocInfo);
        device.bindImageMemory(depthImages[i], depthImageMemories[i], 0);
    }
}

void DepthResources::CreateDepthImageViews() 
{
    for (size_t i = 0; i < depthImages.size(); ++i) 
    {
        vk::ImageViewCreateInfo viewInfo{};
        viewInfo.image = depthImages[i];
        viewInfo.viewType = vk::ImageViewType::e2D;
        viewInfo.format = vk::Format::eD32Sfloat;
        viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        depthImageViews[i] = device.createImageView(viewInfo);
    }
}

void DepthResources::Destroy() 
{
    if (!device) 
    {
        return;
    }

    for (size_t i = 0; i < depthImageViews.size(); ++i) 
    {
        if (depthImageViews[i]) 
        {
            device.destroyImageView(depthImageViews[i]);
            depthImageViews[i] = nullptr;
        }
        
        if (depthImages[i]) 
        {
            device.destroyImage(depthImages[i]);
            depthImages[i] = nullptr;
        }
        
        if (depthImageMemories[i]) 
        {
            device.freeMemory(depthImageMemories[i]);
            depthImageMemories[i] = nullptr;
        }
    }

    depthImageViews.clear();
    depthImages.clear();
    depthImageMemories.clear();

    device = nullptr;        
    physicalDevice = nullptr;
}

const std::vector<vk::Image>& DepthResources::GetImage() const
{
    return depthImages;
}

const std::vector<vk::ImageView>& DepthResources::GetImageView() const
{
    return depthImageViews;
}