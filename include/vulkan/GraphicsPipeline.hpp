#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <string>

class Swapchain; // forward declaration
class VulkanContext; // forward declaration

/**
 * @brief Creates and stores the Vulkan graphics pipeline.
 *
 * This class creates shader modules, shader stages, the render pass,
 * the pipeline layout, and the graphics pipeline.
 *
 * The class uses a borrowed device from VulkanContext.
 * It does not create or destroy the Vulkan device.
 */

class GraphicsPipeline
{
public:
    GraphicsPipeline() = default;
    GraphicsPipeline(const GraphicsPipeline&) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
    ~GraphicsPipeline();

    void Create(const Swapchain& swapchain, const VulkanContext& context);
    void Destroy();

    // getters
    vk::RenderPass GetRenderPass() const;
    vk::Pipeline GetGraphicsPipeline() const;

private:
    std::vector<uint32_t> ReadShaderFile(const std::string& filename) const;
    void CreateShaderModules();
    void CreateShaderStages();
    void CreatePipelineLayout();
    void CreateRenderPass(const Swapchain& swapchain);
    void CreateGraphicsPipeline(); 

    // Create the pipeline state structures and keep them alive
    // while GraphicsPipelineCreateInfo uses pointers to them.
    vk::PipelineVertexInputStateCreateInfo CreateVertexInput();
    vk::PipelineInputAssemblyStateCreateInfo CreateInputAssembly();
    vk::PipelineViewportStateCreateInfo CreateViewportState();
    vk::PipelineRasterizationStateCreateInfo CreateRasterization();
    vk::PipelineMultisampleStateCreateInfo CreateMultisampleState();
    vk::PipelineColorBlendStateCreateInfo CreateColorBlending(vk::PipelineColorBlendAttachmentState& attachment);
    vk::PipelineDynamicStateCreateInfo CreateDynamicState();

private:
    vk::Device device{}; // borrowed handle

    vk::ShaderModule vertexShaderModule{};
    vk::ShaderModule fragmentShaderModule{};

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = std::vector<vk::PipelineShaderStageCreateInfo>(2);
    std::vector<vk::DynamicState> dynamicStates = std::vector<vk::DynamicState>(2);

    vk::RenderPass renderPass{};
    vk::PipelineLayout pipelineLayout{};
    vk::Pipeline graphicsPipeline{};
};
