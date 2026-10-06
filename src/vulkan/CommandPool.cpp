#include "CommandPool.hpp"
#include "VulkanContext.hpp"

CommandPool::~CommandPool()
{
    Destroy();
}

void CommandPool::Create(const VulkanContext& context)
{
    device = context.GetDevice();

    createCommandPool();
    allocateCommandBuffersAndInitializeSyncObject();
}

void CommandPool::Destroy()
{
    if (!device)
    {
        return;
    }

    if(commandPool)
    {
        device.destroyCommandPool(commandPool);
        commandPool = nullptr;
    }

    device = nullptr; // The device is borrowed, so this class does not destroy it
}

void CommandPool::createCommandPool()
{
    vk::CommandPoolCreateInfo createInfoCommandPool{};

    createInfoCommandPool.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    commandPool = device.createCommandPool(createInfoCommandPool);
}

void CommandPool::allocateCommandBuffersAndInitializeSyncObject()
{
    vk::CommandBufferAllocateInfo allocateInfo{};

    allocateInfo.commandPool = commandPool;
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;
    allocateInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

    commandBuffers = device.allocateCommandBuffers(allocateInfo);
}

vk::CommandBuffer CommandPool::GetCommandBuffer(std::uint32_t frameIndex) const
{
    return commandBuffers[frameIndex];
}

