#include "Renderer.hpp"
#include "Swapchain.hpp"
#include "GraphicsPipeline.hpp"
#include "VulkanContext.hpp"
#include "CommandPool.hpp"
#include "SyncObjects.hpp"

void Renderer::Create(const VulkanContext& context, 
    const Swapchain& swapchain,
    const GraphicsPipeline& pipeline, 
    const CommandPool& pool, 
    const SyncObjects& syncObj)
{
    device = context.GetDevice();
    swapChainImageViews = swapchain.GetImageViews();
    SwapChainExtent = swapchain.getExtent();
    renderPass = pipeline.GetRenderPass();
    graphicsPipeline = pipeline.GetGraphicsPipeline();
    SwapChain = swapchain.GetSwapchain();
    graphicsQueue = context.GetGraphicsQueue();
    presentQueue = context.GetPresentQueue();

    for(std::uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        frames[i].CommandBuffer = pool.GetCommandBuffer(i);
        frames[i].ImageAvailableSemaphore = syncObj.getImageAvailableSemaphore(i);
        frames[i].InFlightFence = syncObj.getInFlightFence(i);
    }
    renderFinishedSemaphores.resize(swapchain.GetImageViews().size());
    for(std::size_t i = 0; i < swapchain.GetImageViews().size(); i++)
    {
        renderFinishedSemaphores[i] = syncObj.getRenderFinishedSemaphore(i);
    }

    CreateFrameBuffer();
}

void Renderer::Destroy()
{
    if(!device)
    {
        return;
    }

    // destroy swapChainFramebuffers
    currentFrame = {};
    frames.clear();
    for(size_t i = 0; i < swapChainFramebuffers.size(); i++)
    {
        if(swapChainFramebuffers[i])
        {
           device.destroyFramebuffer(swapChainFramebuffers[i]); 
        }
    }
    swapChainFramebuffers.clear();
    

    // get the null to borowed handles
    renderFinishedSemaphores.clear();
    presentQueue = nullptr;
    graphicsQueue = nullptr;
    SwapChain = nullptr;
    graphicsPipeline = nullptr;
    renderPass = nullptr;
    SwapChainExtent = vk::Extent2D{};
    swapChainImageViews.clear();
    device = nullptr; 
}

void Renderer::CreateFrameBuffer()
{
    // func resize has size_type. the size_type sunonym to size_t
    swapChainFramebuffers.resize(swapChainImageViews.size());

    vk::FramebufferCreateInfo frameBufferInfo{};
    frameBufferInfo.renderPass = renderPass;
    frameBufferInfo.attachmentCount = 1;
    frameBufferInfo.width = SwapChainExtent.width;
    frameBufferInfo.height = SwapChainExtent.height;
    frameBufferInfo.layers = 1;

    for(size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        frameBufferInfo.pAttachments = &swapChainImageViews[i];

        swapChainFramebuffers[i] = device.createFramebuffer(frameBufferInfo);
    }
}

void Renderer::Draw()
{
    vk::CommandBufferBeginInfo beginInfo{};
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    beginInfo.pInheritanceInfo = nullptr; // inheritance's command buffer

    vk::Viewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(SwapChainExtent.width);
    viewport.height = static_cast<float>(SwapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vk::Rect2D rect{};
    rect.offset.x = 0;
    rect.offset.y = 0;
    rect.extent.width = SwapChainExtent.width;
    rect.extent.height = SwapChainExtent.height;

    vk::ClearValue clearColor{};
    clearColor.color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

    vk::RenderPassBeginInfo renderPassInfo{};
    renderPassInfo.renderPass = this->renderPass;
    renderPassInfo.renderArea = rect;
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    std::uint64_t timeout = std::numeric_limits<uint64_t>::max(); 
    (void)device.waitForFences(frames[currentFrame].InFlightFence, true, timeout); 

    auto acquireResult = device.acquireNextImageKHR(SwapChain, timeout, frames[currentFrame].ImageAvailableSemaphore, nullptr);
    if(acquireResult.result == vk::Result::eErrorOutOfDateKHR)
    {
        // recreate swapchain here
        return;
    }
    if(acquireResult.result != vk::Result::eSuccess && acquireResult.result != vk::Result::eSuboptimalKHR)
    {
        throw std::runtime_error("Failed to acquire swapchain image");
    }
    std::uint32_t imageIndex = acquireResult.value;

    device.resetFences(frames[currentFrame].InFlightFence);
    frames[currentFrame].CommandBuffer.reset();

    frames[currentFrame].CommandBuffer.begin(beginInfo);
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    frames[currentFrame].CommandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
    frames[currentFrame].CommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline); // bind pipeline
    frames[currentFrame].CommandBuffer.setViewport(0, viewport); // bind dynamic viewport
    frames[currentFrame].CommandBuffer.setScissor(0, rect); // bind dynamic scissor
    frames[currentFrame].CommandBuffer.draw(3, 1, 0, 0); // call the draw
    frames[currentFrame].CommandBuffer.endRenderPass();
    frames[currentFrame].CommandBuffer.end();

    vk::SemaphoreSubmitInfo waitSemaphoreInf{};
    waitSemaphoreInf.semaphore = frames[currentFrame].ImageAvailableSemaphore;
    waitSemaphoreInf.value = 0;
    waitSemaphoreInf.stageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput;
    waitSemaphoreInf.deviceIndex = 0;
    vk::SemaphoreSubmitInfo signalSemaphoreInf{};
    signalSemaphoreInf.semaphore = renderFinishedSemaphores[imageIndex];
    signalSemaphoreInf.value = 0;
    signalSemaphoreInf.stageMask = vk::PipelineStageFlagBits2::eAllGraphics;
    signalSemaphoreInf.deviceIndex = 0;
    vk::CommandBufferSubmitInfo bufferSubmitInfo{};
    bufferSubmitInfo.commandBuffer = frames[currentFrame].CommandBuffer;
    bufferSubmitInfo.deviceMask = 1;
    vk::SubmitInfo2 submit{};
    submit.waitSemaphoreInfoCount = 1;
    submit.pWaitSemaphoreInfos = &waitSemaphoreInf;
    submit.commandBufferInfoCount = 1;
    submit.pCommandBufferInfos = &bufferSubmitInfo;
    submit.signalSemaphoreInfoCount = 1;
    submit.pSignalSemaphoreInfos = &signalSemaphoreInf;
    graphicsQueue.submit2(submit, frames[currentFrame].InFlightFence);

    vk::PresentInfoKHR presentInfo{};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphores[imageIndex];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &SwapChain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;
    vk::Result presentResult = presentQueue.presentKHR(presentInfo);
    if(presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR)
    {
        // recreate swapchain
        return;
    }
    else if(presentResult != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to present swapchain image");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


