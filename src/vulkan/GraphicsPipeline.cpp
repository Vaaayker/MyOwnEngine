#include "GraphicsPipeline.hpp"
#include "Swapchain.hpp"
#include "VulkanContext.hpp"
#include <stdexcept>
#include <fstream>

GraphicsPipeline::~GraphicsPipeline()
{
    Destroy();
}

void GraphicsPipeline::Create(const Swapchain& swapchain, const VulkanContext& context)
{
    // Intitiize device as a borowed handle
    device = context.GetDevice();

    ReinitializeResources(swapchain);
}

void GraphicsPipeline::ReinitializeResources(const Swapchain& swapchain)
{
    // Shader Module
    CreateShaderModules();

    // Shader Stage
    CreateShaderStages();

    // Create Render Pass
    CreateRenderPass(swapchain);

    // Create Pipeline Layout
    CreatePipelineLayout();

    // Create Graphics Pipeline
    CreateGraphicsPipeline();

}

void GraphicsPipeline::Destroy() // func for shutdown
{
    DestroyResources();

    // The device is borrowed, so this class does not destroy it
    device = nullptr;
}

void GraphicsPipeline::DestroyResources()
{
    if (!device)
    {
        return;
    }

    if (graphicsPipeline)
    {
        device.destroyPipeline(graphicsPipeline);
        graphicsPipeline = nullptr;
    }

    if (pipelineLayout)
    {
        device.destroyPipelineLayout(pipelineLayout);
        pipelineLayout = nullptr;
    }

    if (renderPass)
    {
        device.destroyRenderPass(renderPass);
        renderPass = nullptr;
    }

    if (vertexShaderModule)
    {
        device.destroyShaderModule(vertexShaderModule);
        vertexShaderModule = nullptr;
    }

    if (fragmentShaderModule)
    {
        device.destroyShaderModule(fragmentShaderModule);
        fragmentShaderModule = nullptr;
    }
}

std::vector<uint32_t> GraphicsPipeline::ReadShaderFile(const std::string& filename) const
{
    std::ifstream file;

    file.open(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    const std::streampos endPosition = file.tellg();

    if (endPosition == std::streampos(-1)) // tellg returns -1 if it fails to determine the current position
    {
        throw std::runtime_error("Failed to determine shader file size: " + filename);
    }

    if (endPosition == std::streampos(0)) // the file is empty
    {
        throw std::runtime_error("Shader file is empty: " + filename);
    }

    const std::streamsize fileSize = static_cast<std::streamsize>(endPosition);

    if (fileSize % static_cast<std::streamsize>(sizeof(std::uint32_t)) != 0)
    {
        throw std::runtime_error( "Shader file size is not aligned to 4 bytes: " + filename);
    }

    std::vector<std::uint32_t> code(static_cast<std::size_t>(fileSize) / sizeof(uint32_t));

    file.seekg(0, std::ios::beg); // move the read position to the beginning of the file

    if(!file.read(reinterpret_cast<char*>(code.data()), fileSize))
    {
        throw std::runtime_error("Failed to read shader file: " + filename);
    }

    return code;
}


void GraphicsPipeline::CreateShaderModules()
{
    std::vector<uint32_t> vertexShaderFile = ReadShaderFile("shaders/compiled/triangle.vert.spv");
    vk::ShaderModuleCreateInfo vertexCreateInfo{};
    vertexCreateInfo.codeSize = (vertexShaderFile.size() * sizeof(uint32_t));
    vertexCreateInfo.pCode = vertexShaderFile.data();
    vertexShaderModule = device.createShaderModule(vertexCreateInfo);


    std::vector<uint32_t> fragmentShaderFile = ReadShaderFile("shaders/compiled/triangle.frag.spv");
    vk::ShaderModuleCreateInfo fragmentCreateInfo{};
    fragmentCreateInfo.codeSize = (fragmentShaderFile.size() * sizeof(uint32_t));
    fragmentCreateInfo.pCode = fragmentShaderFile.data();
    fragmentShaderModule = device.createShaderModule(fragmentCreateInfo);
}

void GraphicsPipeline::CreateShaderStages()
{
    vk::PipelineShaderStageCreateInfo vertexShaderStage{};
    vertexShaderStage.stage = vk::ShaderStageFlagBits::eVertex;
    vertexShaderStage.module = vertexShaderModule;
    vertexShaderStage.pName = "main";

    vk::PipelineShaderStageCreateInfo fragmentShaderStage{};
    fragmentShaderStage.stage = vk::ShaderStageFlagBits::eFragment;
    fragmentShaderStage.module = fragmentShaderModule;
    fragmentShaderStage.pName = "main";

    shaderStages = {vertexShaderStage, fragmentShaderStage};
}


vk::PipelineVertexInputStateCreateInfo GraphicsPipeline::CreateVertexInput()
{
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};

    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    return vertexInputInfo;
}

vk::PipelineInputAssemblyStateCreateInfo GraphicsPipeline::CreateInputAssembly()
{
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};

    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = false;

    return inputAssembly;
}

vk::PipelineViewportStateCreateInfo GraphicsPipeline::CreateViewportState()
{
    // dynamic viewport are dynamic state.
    // their values set during recording command buffer
    // in commandBuffer.setViewport() and commandBuffer.setScissor()
    vk::PipelineViewportStateCreateInfo infoState{};

    infoState.viewportCount = 1;
    infoState.pViewports = nullptr;
    infoState.scissorCount = 1;
    infoState.pScissors = nullptr;

    return infoState;
}


vk::PipelineRasterizationStateCreateInfo GraphicsPipeline::CreateRasterization()
{
    vk::PipelineRasterizationStateCreateInfo rasterizer{};

    rasterizer.depthClampEnable = false;
    rasterizer.rasterizerDiscardEnable = false;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable = false;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;
    rasterizer.lineWidth = 1.0f;

    return rasterizer;
}

vk::PipelineMultisampleStateCreateInfo GraphicsPipeline::CreateMultisampleState()
{
    vk::PipelineMultisampleStateCreateInfo multisampleState{};

    multisampleState.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampleState.sampleShadingEnable = false;

    return multisampleState;
}

vk::PipelineColorBlendStateCreateInfo GraphicsPipeline::CreateColorBlending(vk::PipelineColorBlendAttachmentState& attachment)
{
    vk::PipelineColorBlendStateCreateInfo colorBlendState{};

    colorBlendState.logicOpEnable = false;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &attachment;

    return colorBlendState;
}

vk::PipelineDynamicStateCreateInfo GraphicsPipeline::CreateDynamicState()
{
    dynamicStates  = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

    vk::PipelineDynamicStateCreateInfo dynamicStateInfo{};

    dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicStateInfo.pDynamicStates = dynamicStates.data();

    return dynamicStateInfo;
}


void GraphicsPipeline::CreatePipelineLayout()
{
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};

    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);
}

void GraphicsPipeline::CreateRenderPass(const Swapchain& swapchain)
{
    vk::AttachmentDescription colorAttachment{};
    colorAttachment.format = swapchain.getFormat();
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference colorAttachmentReference{};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass{};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentReference;

    vk::RenderPassCreateInfo renderPassInfo{};
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    renderPass = device.createRenderPass(renderPassInfo);
}


void GraphicsPipeline::CreateGraphicsPipeline()
{
    vk::GraphicsPipelineCreateInfo pipelineInfo{};

    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineInfo.pStages = shaderStages.data();

    vk::PipelineVertexInputStateCreateInfo VertexInputState = CreateVertexInput();
    pipelineInfo.pVertexInputState = &VertexInputState;

    vk::PipelineInputAssemblyStateCreateInfo InputAssemblyState = CreateInputAssembly();
    pipelineInfo.pInputAssemblyState = &InputAssemblyState;

    vk::PipelineViewportStateCreateInfo ViewportState = CreateViewportState();
    pipelineInfo.pViewportState = &ViewportState;

    vk::PipelineRasterizationStateCreateInfo Rasterizer = CreateRasterization();
    pipelineInfo.pRasterizationState = &Rasterizer;

    vk::PipelineMultisampleStateCreateInfo multisampleState = CreateMultisampleState();
    pipelineInfo.pMultisampleState = &multisampleState;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = false;
    vk::PipelineColorBlendStateCreateInfo ColorBlendState = CreateColorBlending(colorBlendAttachment);
    pipelineInfo.pColorBlendState = &ColorBlendState;

    vk::PipelineDynamicStateCreateInfo DynamicState = CreateDynamicState();
    pipelineInfo.pDynamicState = &DynamicState;

    pipelineInfo.layout = pipelineLayout;
    
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    graphicsPipeline = device.createGraphicsPipeline(nullptr, pipelineInfo).value;
}

vk::RenderPass GraphicsPipeline::GetRenderPass() const
{
    return renderPass;
}

vk::Pipeline GraphicsPipeline::GetGraphicsPipeline() const
{
    return graphicsPipeline;
}