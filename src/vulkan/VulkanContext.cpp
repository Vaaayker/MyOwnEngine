#include "VulkanContext.hpp"
#include <SDL3/SDL_vulkan.h>
#include "ConfigLayer.hpp"
#include <cstring>
#include <stdexcept>

void VulkanContext::Create(SDL_Window* window)
{
    CreateInstanceAndSurface(window);

#ifndef NDEBUG
    debugMessenger.Create(*this);
#endif

    PickPhysicalDevice();

    GetGraphicQueueProperties();
    GetPresentQueueProperties();

    PickDevice();

    GraphicsQueue();
    PresentQueue();

    mesh.CreateTriangle();
    swapchain.Create(*this, window);
    pool.Create(*this);
    sync.Create(*this, swapchain);
    bufferHelper.Create(mesh, *this);
    pipeline.Create(swapchain, *this, mesh);
    renderer.Create(*this, swapchain, pipeline, pool, sync, bufferHelper, mesh);
}

void VulkanContext::Destroy()
{
    if(device)
    {
        device.waitIdle();
    }

    
    renderer.Destroy();
    bufferHelper.Destroy();
    pipeline.Destroy();
    sync.Destroy();
    pool.Destroy();
    swapchain.Destroy();

    #ifndef NDEBUG
        debugMessenger.Destroy();
    #endif

    if(device)
    {
        device.destroy();
        device = nullptr;
    }

    if(surface)
    {
        instance.destroySurfaceKHR(surface);
        surface = nullptr;
    }

    if(instance)
    {
        instance.destroy();
        instance = nullptr;    
    }
}

void VulkanContext::CreateInstanceAndSurface(SDL_Window* window)
{
    vk::InstanceCreateInfo createInfo{};

    createInfo.sType = vk::StructureType::eInstanceCreateInfo; 
    createInfo.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR; 

    if(enableValidationLayers)
    {
        if(!CheckValidationLayerSupport()) throw std::runtime_error("Validation layers requested, but not available");
 
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();      
    }

    uint32_t extensionCount = 0;
    const char* const* sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

    if (!sdlExtensions) { throw std::runtime_error(SDL_GetError()); }

    std::vector<const char*> extensions(sdlExtensions, sdlExtensions + extensionCount);

    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

    #ifndef NDEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    #endif

    vk::ApplicationInfo appInfo{};
    appInfo.pApplicationName = "My App";
    appInfo.applicationVersion = vk::makeApiVersion(0, 1, 0, 0);
    appInfo.pEngineName = "My Engine";
    appInfo.engineVersion = vk::makeApiVersion(0, 1, 0, 0);
    appInfo.apiVersion = vk::ApiVersion13;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.pApplicationInfo = &appInfo;
    
    instance = vk::createInstance(createInfo);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

    VkSurfaceKHR rawSurface = nullptr;
    
    if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &rawSurface)) { throw std::runtime_error(SDL_GetError()); }

    surface = rawSurface;
}

bool VulkanContext::CheckValidationLayerSupport()
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    for(size_t layerIndex = 0; layerIndex < validationLayers.size(); layerIndex++)
    {
        bool layerFound = false;
        for(size_t availableLayersIndex = 0; availableLayersIndex < availableLayers.size(); availableLayersIndex++)
        {
            if(std::strcmp(validationLayers[layerIndex], availableLayers[availableLayersIndex].layerName) == 0)
            {
                layerFound = true;
                break;
            }  
        }
        if(!layerFound)
        {
            return false;
        }
    }
    return true;
}

void VulkanContext::PickPhysicalDevice()
{
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    if(physicalDevices.empty())
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support");
    }

    physicalDevice = physicalDevices[0];
}



void VulkanContext::GetGraphicQueueProperties()
{
    std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

    for(uint32_t i = 0; i < queueFamilies.size(); i++)
    {
        if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            graphicsQueueFamilyIndex = i;
            return ;
        }
    }

    throw std::runtime_error("Failed to find a graphics queue family");
}

void VulkanContext::GetPresentQueueProperties()
{
    std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

    for(uint32_t i = 0; i < queueFamilies.size(); i++)
    {
        vk::Bool32 presentSupport = physicalDevice.getSurfaceSupportKHR(i, surface);

        if(presentSupport)
        {
            presentQueueFamilyIndex = i;
            return ;
        }
    }

    throw std::runtime_error("Failed to find a present queue family");
}

void VulkanContext::PickDevice()
{
    float queuePriority = 1.0f;

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

    vk::DeviceQueueCreateInfo graphicsQueueCreateInfo{};
    graphicsQueueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    graphicsQueueCreateInfo.queueCount = 1;
    graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;

    queueCreateInfos.push_back(graphicsQueueCreateInfo);

    if (presentQueueFamilyIndex != graphicsQueueFamilyIndex)
    {
        vk::DeviceQueueCreateInfo presentQueueCreateInfo{};
        presentQueueCreateInfo.queueFamilyIndex = presentQueueFamilyIndex;
        presentQueueCreateInfo.queueCount = 1;
        presentQueueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(presentQueueCreateInfo);
    }

    std::vector<const char*> requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME};

    std::vector<vk::ExtensionProperties> availableExtensions = physicalDevice.enumerateDeviceExtensionProperties();

    bool portabilitySubsetSupported = false;

    for (const auto& extension : availableExtensions)
    {
        if (std::strcmp(extension.extensionName,"VK_KHR_portability_subset") == 0)
        {
            portabilitySubsetSupported = true;
            break;
        }
    }

    if (portabilitySubsetSupported)
    {
        requiredDeviceExtensions.push_back("VK_KHR_portability_subset");
    }

    vk::PhysicalDeviceSynchronization2Features synchronization2Features{};
    synchronization2Features.synchronization2 = vk::True;

    vk::DeviceCreateInfo deviceCreateInfo{};

    deviceCreateInfo.pNext = &synchronization2Features;

    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size());

    deviceCreateInfo.ppEnabledExtensionNames =requiredDeviceExtensions.data();

    deviceCreateInfo.pEnabledFeatures = nullptr;

    device = physicalDevice.createDevice(deviceCreateInfo);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(device);
}

void VulkanContext::GraphicsQueue()
{
    graphicsQueue = device.getQueue(graphicsQueueFamilyIndex, 0);
}

void VulkanContext::PresentQueue()
{
    presentQueue = device.getQueue(presentQueueFamilyIndex, 0);
}

vk::Instance VulkanContext::GetInstance() const
{
    return instance;
}

vk::SurfaceKHR VulkanContext::GetSurface() const
{
    return surface;
}

vk::PhysicalDevice VulkanContext::GetPhysicalDevice() const
{
    return physicalDevice;
}

vk::Device VulkanContext::GetDevice() const
{
    return device;
}

std::uint32_t VulkanContext::GetGraphicsQueueFamilyIndex() const
{
    return graphicsQueueFamilyIndex;
}

std::uint32_t VulkanContext::GetPresentQueueFamilyIndex() const
{
    return presentQueueFamilyIndex;
}

vk::Queue VulkanContext::GetGraphicsQueue() const
{
    return graphicsQueue;
}

vk::Queue VulkanContext::GetPresentQueue() const
{
    return presentQueue;
}

void VulkanContext::MakeDraw(SDL_Window* window)
{
    renderer.CallDraw();

    if((renderer.GetErrorOutOfDate()) || (renderer.GetSuboptimal()))
    {
        int width = 0;
        int height = 0;

        SDL_GetWindowSizeInPixels(window, &width, &height);

        if (width == 0 || height == 0)
        {
            throw std::runtime_error("The window is minimized or hasn't drawable-size ");
        }

        device.waitIdle();

        renderer.DestroyResources(); // DestroyResources
        pipeline.DestroyResources();
        sync.DestroyResources();

        swapchain.Recreate(window);

        sync.ReinitializeResources(swapchain); // ReinitiliazeResources
        pipeline.ReinitializeResources(swapchain, mesh);
        renderer.ReinitializeResources(swapchain, sync, pipeline);

        renderer.ResetFlags();
    }
}

