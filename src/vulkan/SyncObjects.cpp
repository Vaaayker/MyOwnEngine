#include "SyncObjects.hpp"
#include "VulkanContext.hpp"
#include "FrameResources.hpp"
#include "Swapchain.hpp"


SyncObjects::~SyncObjects()
{
    Destroy();
}

void SyncObjects::Create(const VulkanContext& context, const Swapchain& swapchain)
{
    device = context.GetDevice();
    renderFinishedCount = swapchain.GetImageViews().size();
    renderFinishedSemaphores.resize(renderFinishedCount);

    createSemaphores();
    createFences();
}

void SyncObjects::ReinitializeResources(const Swapchain& swapchain)
{
    vk::SemaphoreCreateInfo semaphoreInfo{};

    renderFinishedCount = swapchain.GetImageViews().size();
    renderFinishedSemaphores.resize(renderFinishedCount);

    for(std::size_t i = 0; i < renderFinishedCount; i++)
    {
        renderFinishedSemaphores[i] = device.createSemaphore(semaphoreInfo);
    }
}
    
void SyncObjects::DestroyResources()
{
    for(std::uint32_t i = 0; i < renderFinishedCount; i++)
    {
        if (renderFinishedSemaphores[i])
        {
            device.destroySemaphore(renderFinishedSemaphores[i]);
            renderFinishedSemaphores[i] = nullptr;
        }
    }

    renderFinishedSemaphores.clear();
}

void SyncObjects::Destroy()
{
    if(!device)
    {
        return;
    }

    for(std::uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (imageAvailableSemaphores[i])
        {
            device.destroySemaphore(imageAvailableSemaphores[i]);
            imageAvailableSemaphores[i] = nullptr;
        }
        
        if (inFlightFences[i])
        {
            device.destroyFence(inFlightFences[i]);
            inFlightFences[i] = nullptr;
        }
    }

    DestroyResources();

    imageAvailableSemaphores.clear();
    inFlightFences.clear();

    device = nullptr; // The device is borrowed, so this class does not destroy it
}

void SyncObjects::createSemaphores()
{
    vk::SemaphoreCreateInfo semaphoreInfo{};

    for(std::uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        imageAvailableSemaphores[i] = device.createSemaphore(semaphoreInfo);
    }
    for(std::size_t i = 0; i < renderFinishedCount; i++)
    {
        renderFinishedSemaphores[i] = device.createSemaphore(semaphoreInfo);
    }
}
    
void SyncObjects::createFences()
{
    vk::FenceCreateInfo fenceInfo{};
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        inFlightFences[i] = device.createFence(fenceInfo);
    }
}

vk::Semaphore SyncObjects::getImageAvailableSemaphore(std::uint32_t frameIndex) const
{
    return imageAvailableSemaphores[frameIndex];
}

const std::vector<vk::Semaphore>& SyncObjects::getRenderFinishedSemaphore() const
{
    return renderFinishedSemaphores;
}

vk::Fence SyncObjects::getInFlightFence(std::uint32_t frameIndex) const
{
    return inFlightFences[frameIndex];
}

