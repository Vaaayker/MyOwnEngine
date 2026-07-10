#pragma once

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>

#include <cstdint>
#include <vector>

#include "Swapchain.hpp"
#include "DebugMessanger.hpp"

class VulkanContext
{
public:
    // func for vulkan context
    VulkanContext() = default;
    ~VulkanContext();

    void Create(SDL_Window* window);
    void Destroy();

    void CreateInstanceAndSurface(SDL_Window* window);
    bool CheckValidationLayerSupport();

    void PickPhysicalDevice();

    int32_t GetGraphicQueueProperties();
    int32_t GetPresentQueueProperties();

    void PickDevice();

    void GraphicsQueue();
    void PresentQueue();

    // getters 
    vk::Instance GetInstance() const;
    vk::SurfaceKHR GetSurface() const;
    vk::PhysicalDevice GetPhysicalDevice() const;
    vk::Device GetDevice() const;

private:
    vk::Instance instance{};
    vk::SurfaceKHR surface{};

    vk::PhysicalDevice physicalDevice{};
    vk::Device device{};

    vk::Queue graphicsQueue{};
    vk::Queue presentQueue{};

    Swapchain swapchain{};

#ifndef NDEBUG
    DebugMessanger debugMessenger{};
#endif

};