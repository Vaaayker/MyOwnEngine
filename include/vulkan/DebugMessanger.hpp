#pragma once
#include <vulkan/vulkan.hpp>

class VulkanContext; // forward declaration

class DebugMessanger
{
public:
    DebugMessanger() = default;
    ~DebugMessanger();

    void Create(const VulkanContext& context);
    void Destroy();

private:
    vk::Instance instance{}; // borowed handle. owned by VulkanContext class

    vk::DebugUtilsMessengerEXT messenger{}; // owned by this class
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* callbackData,
        void* userData
    );

};