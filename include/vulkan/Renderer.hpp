#pragma once
#include <vulkan/vulkan.hpp>
#include "FrameResources.hpp"

// forward declarations:
class Swapchain; 
class GraphicsPipeline; 
class VulkanContext; 
class CommandPool; 
class SyncObjects; 
struct FrameResources; 

/**
 * @brief Manages frame rendering.
 *
 * Creates framebuffers, records drawing commands, submits them to the graphics
 * queue, and presents rendered images to the screen.
 */

class Renderer
{
public:
    Renderer() = default;

    void Create(const VulkanContext& context,
        const Swapchain& swapchain, 
        const GraphicsPipeline& pipeline, 
        const CommandPool& pool,
        const SyncObjects& syncObj);
    void Destroy();
    
    void Draw();
    void CreateFrameBuffer();


private:
    // borowed handles:
    vk::Device device{};
    std::vector<vk::ImageView> swapChainImageViews{}; 
    vk::Extent2D SwapChainExtent{}; 
    vk::RenderPass renderPass{};
    vk::Pipeline graphicsPipeline{};
    vk::SwapchainKHR SwapChain{};
    vk::Queue graphicsQueue{};
    vk::Queue presentQueue{};
    std::vector<vk::Semaphore> renderFinishedSemaphores;

    // owned by this class:
    std::vector<vk::Framebuffer> swapChainFramebuffers;
    std::vector<FrameResources> frames = std::vector<FrameResources>(MAX_FRAMES_IN_FLIGHT); 
    std::uint32_t currentFrame{};
};