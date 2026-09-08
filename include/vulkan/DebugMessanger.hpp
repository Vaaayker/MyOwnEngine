#pragma once
#include <vulkan/vulkan.hpp>

class VulkanContext; // forward declaration

/**
 * @brief Manages the Vulkan debug messenger.
 *
 * Creates and owns a debug messenger used to receive validation layer
 * messages. The Vulkan instance is borrowed from VulkanContext.
 */

class DebugMessanger
{
public:
    DebugMessanger() = default;
    DebugMessanger(const DebugMessanger&) = delete;
    DebugMessanger& operator=(const DebugMessanger&) = delete;
    ~DebugMessanger();

    void Create([[maybe_unused]]const VulkanContext& context);
    void Destroy();

private:
    vk::Instance instance{}; // borowed handle. owned by VulkanContext class

    vk::DebugUtilsMessengerEXT messenger{}; // owned by this class:
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        [[maybe_unused]] vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        [[maybe_unused]] vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* callbackData,
        [[maybe_unused]] void* userData
    );

};