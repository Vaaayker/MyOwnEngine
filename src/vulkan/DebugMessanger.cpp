#include "DebugMessanger.hpp"
#include "VulkanContext.hpp"
#include <iostream>
#include <stdexcept>


DebugMessanger::~DebugMessanger()
{
    Destroy();
}

void DebugMessanger::Create(const VulkanContext& context)
{
    #ifndef NDEBUG
    instance = context.GetInstance();

    vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.messageSeverity =
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;

    createInfo.messageType =
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

    createInfo.pfnUserCallback = &DebugMessanger::debugCallback;

    messenger = instance.createDebugUtilsMessengerEXT(createInfo);
#endif
}

void DebugMessanger::Destroy()
{
    #ifndef NDEBUG
    if (messenger)
    {
        instance.destroyDebugUtilsMessengerEXT(messenger);
        messenger = nullptr;
    }
    #endif
}



VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessanger::debugCallback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
    vk::DebugUtilsMessageTypeFlagsEXT type,
    const vk::DebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData
)
{
    std::cerr << "validation layer: " << callbackData->pMessage << std::endl;
    return VK_FALSE;
}

