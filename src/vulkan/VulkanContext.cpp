#include "VulkanContext.hpp"
#include <SDL3/SDL_vulkan.h>
#include "ConfigLayer.hpp"
#include <cstring>
#include <stdexcept>

VulkanContext::~VulkanContext()
{
    Destroy();
}

void VulkanContext::Create(SDL_Window* window)
{
    CreateInstanceAndSurface(window);

    PickPhysicalDevice();
    GetGraphicQueueProperties();

    GetPresentQueueProperties();
    PickDevice();

    GraphicsQueue();
    PresentQueue();

    swapchain.Create(*this, window);

    #ifndef NDEBUG
        debugMessenger.Create(*this);
    #endif
}

void VulkanContext::Destroy()
{
    if(device)
    {
        device.waitIdle();
    }

    #ifndef NDEBUG
        debugMessenger.Destroy();
    #endif

    swapchain.Destroy();

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

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
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



int32_t VulkanContext::GetGraphicQueueProperties()
{
    std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

    for(uint32_t i = 0; i < queueFamilies.size(); i++)
    {
        if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            return static_cast<int32_t>(i);
        }
    }

    return -1;
}

int32_t VulkanContext::GetPresentQueueProperties()
{
    std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

    for(uint32_t i = 0; i < queueFamilies.size(); i++)
    {
        vk::Bool32 presentSupport = physicalDevice.getSurfaceSupportKHR(i, surface);

        if(presentSupport)
        {
            return static_cast<uint32_t>(i);
        }
    }

    return -1;
}

void VulkanContext::PickDevice()
{
    int32_t graphicsFamilyIndex = GetGraphicQueueProperties();
    int32_t presentFamilyIndex = GetPresentQueueProperties();

    if (graphicsFamilyIndex == -1)
    {
        throw std::runtime_error("Failed to find graphics queue family");
    }

    if (presentFamilyIndex == -1)
    {
        throw std::runtime_error("Failed to find present queue family");
    }

    float queuePriority = 1.0f;

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

    vk::DeviceQueueCreateInfo graphicsQueueCreateInfo{};
    graphicsQueueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(graphicsFamilyIndex);
    graphicsQueueCreateInfo.queueCount = 1;
    graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;

    queueCreateInfos.push_back(graphicsQueueCreateInfo);

    if (presentFamilyIndex != graphicsFamilyIndex)
    {
        vk::DeviceQueueCreateInfo presentQueueCreateInfo{};
        presentQueueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(presentFamilyIndex);
        presentQueueCreateInfo.queueCount = 1;
        presentQueueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(presentQueueCreateInfo);
    }

    std::vector<const char*> requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    requiredDeviceExtensions.push_back("VK_KHR_portability_subset");

    vk::DeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();

    deviceCreateInfo.pEnabledFeatures = nullptr;

    device = physicalDevice.createDevice(deviceCreateInfo);
}

void VulkanContext::GraphicsQueue()
{
    uint32_t graphicsFamilyIndex = static_cast<uint32_t>(GetGraphicQueueProperties());

    graphicsQueue = device.getQueue(graphicsFamilyIndex, 0);
}

void VulkanContext::PresentQueue()
{
    uint32_t presentFamilyIndex = static_cast<uint32_t>(GetPresentQueueProperties());

    presentQueue = device.getQueue(presentFamilyIndex, 0);
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

