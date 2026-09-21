#pragma once
#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>
#include <cstdint>
#include <vector>
#include "DebugMessanger.hpp"
#include "Swapchain.hpp"
#include "CommandPool.hpp"
#include "SyncObjects.hpp"
#include "GraphicsPipeline.hpp"
#include "Renderer.hpp"

/**
 * @brief Manages the main Vulkan objects and device context.
 *
 * Creates and owns the Vulkan instance, surface, physical device, logical
 * device, queues, swapchain, and debug messenger used by the renderer.
 */

class VulkanContext
{
public:
    VulkanContext() = default;
    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;    

    void Create(SDL_Window* window);
    void Destroy();
    
    void MakeDraw(SDL_Window* window);

    // getters 
    vk::Instance GetInstance() const;
    vk::SurfaceKHR GetSurface() const;
    vk::PhysicalDevice GetPhysicalDevice() const;
    vk::Device GetDevice() const;
    std::uint32_t GetGraphicsQueueFamilyIndex() const;
    std::uint32_t GetPresentQueueFamilyIndex() const;
    vk::Queue GetGraphicsQueue() const;
    vk::Queue GetPresentQueue() const;

private:
    void CreateInstanceAndSurface(SDL_Window* window);
    bool CheckValidationLayerSupport();

    void PickPhysicalDevice();

    void PickDevice();

    void GraphicsQueue();
    void PresentQueue();

    void GetGraphicQueueProperties();
    void GetPresentQueueProperties();

private:
    vk::Instance instance{};
    vk::SurfaceKHR surface{};

    vk::PhysicalDevice physicalDevice{};
    vk::Device device{};

    std::uint32_t graphicsQueueFamilyIndex{};
    std::uint32_t presentQueueFamilyIndex{};

    vk::Queue graphicsQueue{};
    vk::Queue presentQueue{};

#ifndef NDEBUG
    DebugMessanger debugMessenger{};
#endif

    Renderer renderer{};
    Swapchain swapchain{};
    CommandPool pool{};
    SyncObjects sync{};
    GraphicsPipeline pipeline{};
    

};