#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <string>
#include "FrameResources.hpp"

class Swapchain; // forward declaration
class VulkanContext; // forward declaration
class Mesh; // forward declaration
class BufferHelper; // forward declaration

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

    void Create(const Swapchain& swapchain, const VulkanContext& context, const Mesh& mesh, const BufferHelper& bufferHelper);
    void ReinitializeResources(const Swapchain& swapchain, const Mesh& mesh);
    void Destroy();
    void DestroyResources();

    // getters
    vk::RenderPass GetRenderPass() const;
    vk::Pipeline GetGraphicsPipeline() const;
    vk::PipelineLayout GetPipelineLayout() const;
    const std::vector<vk::DescriptorSet>& GetDescriptorSets() const;

private:
    std::vector<uint32_t> ReadShaderFile(const std::string& filename) const;
    void CreateShaderModules();
    void CreateShaderStages();
    void CreatePipelineLayout();
    void CreateRenderPass(const Swapchain& swapchain);
    void CreateGraphicsPipeline(const Swapchain& swapchain, const Mesh& mesh);
    void CreateDescriptorPool(vk::DescriptorPoolSize& poolSize);
    void AllocateDescriptorSets();
    void CreateDescriptorSetLayout(vk::DescriptorSetLayoutBinding& uboLayoutBinding);
    // descriptors don't depend on swapchain, so they live from Create() to Destroy()
    void CreateDescriptors(const BufferHelper& bufferHelper);
    void UpdateDescriptorSets(const BufferHelper& bufferHelper);
    void DestroyDescriptors();
    // Create the pipeline state structures and keep them alive
    // while GraphicsPipelineCreateInfo uses pointers to them.
    vk::PipelineVertexInputStateCreateInfo CreateVertexInput(std::vector<vk::VertexInputAttributeDescription>& attributeDescriptions, vk::VertexInputBindingDescription& bindingDescription);
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
    std::vector<vk::VertexInputAttributeDescription> attributeDescription;
    std::vector<vk::AttachmentDescription> attachmentDescriptions{};
    std::vector<vk::SubpassDescription> subpassDescriptions{};
    std::array<vk::DescriptorSetLayout, MAX_FRAMES_IN_FLIGHT> layouts{};
    
    vk::RenderPass renderPass{};
    vk::PipelineLayout pipelineLayout{};
    vk::Pipeline graphicsPipeline{};
    vk::DescriptorPool DescriptorPool{};
    std::vector<vk::DescriptorSet> DescriptorSets{};
    vk::DescriptorSetLayout DescriptorSetLayout;
};
