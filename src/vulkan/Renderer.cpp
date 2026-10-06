#include "Renderer.hpp"
#include "Swapchain.hpp"
#include "GraphicsPipeline.hpp"
#include "VulkanContext.hpp"
#include "CommandPool.hpp"
#include "SyncObjects.hpp"
#include "BufferHelper.hpp"
#include "Mesh.hpp"

void Renderer::Create(const VulkanContext& context, 
    const Swapchain& swapchain,
    const GraphicsPipeline& pipeline, 
    const CommandPool& pool, 
    const SyncObjects& syncObj,
    const BufferHelper& bufferHelper,
    const Mesh& mesh)
{
    device = context.GetDevice();
    swapChainImageViews = swapchain.GetImageViews();
    SwapChainExtent = swapchain.getExtent();
    renderPass = pipeline.GetRenderPass();
    graphicsPipeline = pipeline.GetGraphicsPipeline();
    SwapChain = swapchain.GetSwapchain();
    graphicsQueue = context.GetGraphicsQueue();
    presentQueue = context.GetPresentQueue();
    Vertices = mesh.GetVertices();
    sizeVertices = static_cast<std::uint32_t>(Vertices.size() * sizeof(Vertices[0]));
    VertexBuffer = bufferHelper.GetVertexBuffer();
    IndicesBuffer = bufferHelper.GetIndicesBuffer();
    StageVertexBuffer = bufferHelper.GetStageVertexBuffer();
    StageIndicesBuffer = bufferHelper.GetStageIndicesBuffer();
    sizeIndices = static_cast<std::uint32_t>(mesh.GetIndices().size() * sizeof(mesh.GetIndices()[0]));

    for(std::uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        frames[i].CommandBuffer = pool.GetCommandBuffer(i); 
        frames[i].ImageAvailableSemaphore = syncObj.getImageAvailableSemaphore(i); 
        frames[i].InFlightFence = syncObj.getInFlightFence(i); 
    }
    
    renderFinishedSemaphores.resize(swapchain.GetImageViews().size());
    renderFinishedSemaphores = syncObj.getRenderFinishedSemaphore(); 

    CreateFrameBuffer();
}

void Renderer::ReinitializeResources(const Swapchain& swapchain,
        const SyncObjects& syncObj,
        const GraphicsPipeline& pipeline)
{
    // SwapChain
    SwapChain = swapchain.GetSwapchain();

    // swapChainImageViews
    swapChainImageViews = swapchain.GetImageViews();

    // SwapChainExtent
    SwapChainExtent = swapchain.getExtent();

    // renderPass
    renderPass = pipeline.GetRenderPass();

    // graphicsPipeline
    graphicsPipeline = pipeline.GetGraphicsPipeline();

    // renderFinishedSemaphores 
    renderFinishedSemaphores.resize(swapchain.GetImageViews().size());
    renderFinishedSemaphores = syncObj.getRenderFinishedSemaphore();

    CreateFrameBuffer();
}

void Renderer::DestroyResources()
{
    if(!device)
    {
        return;
    }

    for(size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        device.destroyFramebuffer(swapChainFramebuffers[i]);
        swapChainFramebuffers[i] = nullptr;
    }

    renderFinishedSemaphores.clear(); 
    graphicsPipeline = nullptr;
    renderPass = nullptr;
    SwapChainExtent = vk::Extent2D{};
    swapChainImageViews.clear();
    SwapChain = nullptr;
    // VertexBuffer = nullptr;
}

void Renderer::Destroy()
{
    currentFrame = {};
    frames.clear();

    DestroyResources();

    // get the null to borowed handles
    presentQueue = nullptr;
    graphicsQueue = nullptr;
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
    if (!buffersUploaded)
    {
        auto& commandBuffer = frames[0].CommandBuffer;

        commandBuffer.reset();

        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        commandBuffer.begin(beginInfo);

        vk::BufferCopy vertexCopy{};
        vertexCopy.srcOffset = 0;
        vertexCopy.dstOffset = 0;
        vertexCopy.size = sizeVertices;

        commandBuffer.copyBuffer(
            StageVertexBuffer,
            VertexBuffer,
            vertexCopy
        );

        vk::BufferCopy indexCopy{};
        indexCopy.srcOffset = 0;
        indexCopy.dstOffset = 0;
        indexCopy.size = sizeIndices;

        commandBuffer.copyBuffer(
            StageIndicesBuffer,
            IndicesBuffer,
            indexCopy
        );

        commandBuffer.end();

        vk::CommandBufferSubmitInfo commandBufferInfo{};
        commandBufferInfo.commandBuffer = commandBuffer;
        commandBufferInfo.deviceMask = 1;

        vk::SubmitInfo2 submit{};
        submit.commandBufferInfoCount = 1;
        submit.pCommandBufferInfos = &commandBufferInfo;

        graphicsQueue.submit2(submit);

        graphicsQueue.waitIdle();

        buffersUploaded = true;
    }

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
    auto waitFenceResult = device.waitForFences(frames[currentFrame].InFlightFence, true, timeout); 
    if(waitFenceResult == vk::Result::eTimeout)
    {
        throw std::runtime_error("Get the eTimeout from func waitForFences");
    }

    auto acquireResult = device.acquireNextImageKHR(SwapChain, timeout, frames[currentFrame].ImageAvailableSemaphore, nullptr);
    if(acquireResult.result == vk::Result::eErrorOutOfDateKHR)
    {
        eErrorOutOfDate = true; 
        return; // return for recreate swapchain
    }
    if(acquireResult.result == vk::Result::eSuboptimalKHR)
    {
        eSubOptimal = true;
    }
    if(acquireResult.result == vk::Result::eTimeout)
    {
        throw std::runtime_error("Get the eTimeout from func acquireNextImageKHR");
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
    frames[currentFrame].CommandBuffer.bindVertexBuffers(0, VertexBuffer, {0}); // bind vertex buffer
    frames[currentFrame].CommandBuffer.bindIndexBuffer(IndicesBuffer, 0, vk::IndexType::eUint32); // bind index buffer
    frames[currentFrame].CommandBuffer.drawIndexed(3, 1, 0, 0, 0); // call the draw indexed
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
    if(presentResult == vk::Result::eErrorOutOfDateKHR)
    {
        eErrorOutOfDate = true; 
        return; // return for recreate swapchain
    }
    if(presentResult == vk::Result::eSuboptimalKHR)
    {
        eSubOptimal = true;
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::CallDraw()
{
    Draw();
}

bool Renderer::GetErrorOutOfDate() const
{
    return eErrorOutOfDate;
}

bool Renderer::GetSuboptimal() const
{
    return eSubOptimal;
}

void Renderer::ResetFlags()
{
    eErrorOutOfDate = false;
    eSubOptimal = false;
}




