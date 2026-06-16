#include "VulkanContext.hpp"
#include "vector"
#include <SDL3/SDL_vulkan.h>
#include "ConfigLayer.hpp"
using namespace std;

void VulkanContext::CreateInstanceAndSurface(SDL_Window* window)
{
    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    createInfo.pApplicationInfo = nullptr;

    bool result = CheckValidationLayerSupport();

    if(enableValidationLayers && result)
    {
        uint32_t layerCount = validationLayers.size();
        createInfo.enabledLayerCount = layerCount;
        createInfo.ppEnabledLayerNames = validationLayers.data();      
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
    }

    uint32_t extensionCount = 0;
    const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;
    
    vkCreateInstance(&createInfo, nullptr, &instance);

    SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface);
}

bool VulkanContext::CheckValidationLayerSupport()
{
    uint32_t layerCount = 0;

    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    vector<VkLayerProperties> availableLayers(layerCount);

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    bool layerFound = false;

    for(int i = 0; i < layerCount; i++)
    {
        if(strcmp(validationLayers[0], availableLayers[i].layerName) == 0)
        {
            layerFound = true;
            break;
        }        
    }

    if(layerFound)
    {
        return true;
    }

    return false;
}

void VulkanContext::PickPhysicalDevice()
{
    uint32_t physicalDeviceCount = 1;
    VkPhysicalDevice *pPhysicalDevices;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    vector<VkPhysicalDevice> physicalDevice(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevice.data());
}



int32_t VulkanContext::GetGraphicQueueProperties()
{
    VkPhysicalDevice physicalDevice; 

    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    uint32_t graphicsFamilyIndex = 0;

    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphicsFamilyIndex = i;
            return i;
        }
    }

    return 0;
}

int32_t VulkanContext::GetPresentQueueProperties()
{
    VkPhysicalDevice physicalDevice;
    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    uint32_t presentFamilyIndex = 0;
    VkBool32 presentSupport = false;

    for(int32_t i = 0; i < queueFamilyCount; i++)
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

        if(presentSupport)
        {
            presentFamilyIndex = i;
            return i;
        }
    }

    return 0;
}

void VulkanContext::PickDevice()
{
    VkDeviceQueueCreateInfo queueCreateInfo{};

    float queuePriority = 1.0f;
    uint32_t graphicsFamilyIndex = GetGraphicQueueProperties(); 

    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = nullptr;
    queueCreateInfo.flags = 0;
    queueCreateInfo.queueFamilyIndex = graphicsFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;


    VkDeviceCreateInfo deviceCreateInfo{};

    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.ppEnabledExtensionNames = nullptr;
    deviceCreateInfo.ppEnabledExtensionNames = 0;
    deviceCreateInfo.pEnabledFeatures = nullptr;

    vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);
}

void VulkanContext::GraphicsQueue()
{
    uint32_t graphicsFamilyIndex = GetGraphicQueueProperties();

    vkGetDeviceQueue(device, graphicsFamilyIndex, 0, &graphicsQueue);
}

void VulkanContext::PresentQueue()
{
    uint32_t graphicsFamilyIndex = GetPresentQueueProperties();

    vkGetDeviceQueue(device, graphicsFamilyIndex, 0, &presentQueue);
}

VulkanContext::VulkanContext(SDL_Window* window)
{
    CreateInstanceAndSurface(window);
    CheckValidationLayerSupport();
    PickPhysicalDevice();
    GetGraphicQueueProperties();
    GetPresentQueueProperties();
    PickDevice();
    GraphicsQueue();
    PresentQueue();
}