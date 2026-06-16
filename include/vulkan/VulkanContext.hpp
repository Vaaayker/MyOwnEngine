#pragma once
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>

class VulkanContext
{
private:
    VkSurfaceKHR surface;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

public:
    void CreateInstanceAndSurface(SDL_Window* window);
    bool CheckValidationLayerSupport();
    void PickPhysicalDevice();
    int32_t GetGraphicQueueProperties();
    int32_t GetPresentQueueProperties();
    void PickDevice();
    void GraphicsQueue();
    void PresentQueue();
    VulkanContext(SDL_Window* window);
};