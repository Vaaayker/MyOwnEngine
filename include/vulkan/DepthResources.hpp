#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <vector>

class VulkanContext;
class Swapchain;

class DepthResources {
public:
    void CreateDepthResources(const VulkanContext& context, const Swapchain& swapchain);
    void AllocateDepthImagesMemory();
    void CreateDepthImageViews();
    std::uint32_t FindMemoryTypeIndex(vk::PhysicalDeviceMemoryProperties propertiesDevice, vk::MemoryRequirements requirements, vk::MemoryPropertyFlags requiredProperties);
    void Destroy();

    // Геттери приймають індекс поточного кадру
    const std::vector<vk::Image>& GetImage() const;
    const std::vector<vk::ImageView>& GetImageView() const;

private:
    vk::Device device{};
    vk::PhysicalDevice physicalDevice{};

    // Вектори для кожного кадру в польоті
    std::vector<vk::Image> depthImages{};
    std::vector<vk::DeviceMemory> depthImageMemories{};
    std::vector<vk::ImageView> depthImageViews{};
};