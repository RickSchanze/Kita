//
// Created by kita on 25-8-26.
//

#include "Pipeline_Vulkan.h"

#include "GfxContext_Vulkan.h"
#include "RHI/DescriptorSet.h"
#include "RHI/RenderPass.h"
#include "RHI/ShaderModule.h"
#include "RHIEnums_Vulkan.h"

RHIPipelineLayout_Vulkan::RHIPipelineLayout_Vulkan(const RHIPipelineLayoutDesc& Desc) {
  VkPipelineLayoutCreateInfo CreateInfo = {};
  auto Layouts =                                                                                                                               //
      Desc.Layouts |                                                                                                                           //
      Ranges::Views::Transform([](const RHIDescriptorSetLayout* NewLayout) { return NewLayout->GetNativeHandleT<VkDescriptorSetLayout>(); }) | //
      Ranges::To<Array<VkDescriptorSetLayout>>();
  CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  CreateInfo.setLayoutCount = static_cast<uint32_t>(Layouts.Count());
  CreateInfo.pSetLayouts = Layouts.Data();
  if (const VkResult Result = vkCreatePipelineLayout(GetVulkanGfxContexRef().GetDevice(), &CreateInfo, nullptr, &mLayout); Result != VK_SUCCESS) {
    mLayout = nullptr;
    LOG_ERROR_TAG("RHI.Vulkan", "创建管线失败,  Code={}", Result);
  }
}

RHIPipelineLayout_Vulkan::~RHIPipelineLayout_Vulkan() {
  if (mLayout) {
    vkDestroyPipelineLayout(GetVulkanGfxContexRef().GetDevice(), mLayout, nullptr);
  }
}

RHIPipeline_Vulkan::RHIPipeline_Vulkan(const RHIGraphicsPipelineDesc& Desc) {
  VkGraphicsPipelineCreateInfo PipelineCreateInfo = {};
  // ShaderStages
  Array<VkPipelineShaderStageCreateInfo> ShaderStages = Desc.ShaderStages | Ranges::Views::Transform([](const RHIPipelineShaderStageDesc& ShaderStage) {
                                                          VkPipelineShaderStageCreateInfo CreateInfo = {};
                                                          CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                                                          CreateInfo.stage = RHIShaderStageBitToVkShaderStageBit(ShaderStage.ShaderStage);
                                                          CreateInfo.module = ShaderStage.ShaderModule->GetNativeHandleT<VkShaderModule>();
                                                          CreateInfo.pName = ShaderStage.EntryName;
                                                          return CreateInfo;
                                                        }) |
                                                        Ranges::To<Array<VkPipelineShaderStageCreateInfo>>();

  PipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  PipelineCreateInfo.stageCount = ShaderStages.Count();
  PipelineCreateInfo.pStages = ShaderStages.Data();

  VkPipelineVertexInputStateCreateInfo VertexInputState = {};
  VertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  auto BindingDescriptions = //
      Desc.VertexInput.VertexBindings | Ranges::Views::Transform([](const RHIVertexInputBindingDesc& BindingDesc) {
        VkVertexInputBindingDescription CreateInfo = {};
        CreateInfo.binding = BindingDesc.Binding;
        CreateInfo.stride = BindingDesc.Stride;
        CreateInfo.inputRate = RHIVertexInputRateToVkVertexInputRate(BindingDesc.InputRate);
        return CreateInfo;
      }) |
      Ranges::To<Array<VkVertexInputBindingDescription>>();
  auto AttributeDescriptions = //
      Desc.VertexInput.VertexAttributes | Ranges::Views::Transform([](const RHIVertexInputAttributeDesc& AttributeDesc) {
        VkVertexInputAttributeDescription CreateInfo = {};
        CreateInfo.location = AttributeDesc.Location;
        CreateInfo.binding = AttributeDesc.Binding;
        CreateInfo.format = RHIFormatToVkFormat(AttributeDesc.Format);
        CreateInfo.offset = AttributeDesc.Offset;
        return CreateInfo;
      }) |
      Ranges::To<Array<VkVertexInputAttributeDescription>>();
  VertexInputState.vertexAttributeDescriptionCount = AttributeDescriptions.Count();
  VertexInputState.vertexBindingDescriptionCount = BindingDescriptions.Count();
  VertexInputState.pVertexAttributeDescriptions = AttributeDescriptions.Data();
  VertexInputState.pVertexBindingDescriptions = BindingDescriptions.Data();
  PipelineCreateInfo.pVertexInputState = &VertexInputState;

  VkPipelineInputAssemblyStateCreateInfo InputAssemblyState = {};
  InputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  InputAssemblyState.topology = RHIPrimitiveTopologyToVkPrimitiveTopology(Desc.InputAssembly.Topology);
  InputAssemblyState.primitiveRestartEnable = false;
  PipelineCreateInfo.pInputAssemblyState = &InputAssemblyState;

  VkPipelineViewportStateCreateInfo ViewportState = {};
  ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  auto Viewports = //
      Desc.Viewport.Viewports | Ranges::Views::Transform([](const RHIPipelineViewport& Viewport) {
        VkViewport CreateInfo = {};
        CreateInfo.x = Viewport.Position.X();
        CreateInfo.y = Viewport.Position.Y();
        CreateInfo.width = Viewport.Size.X();
        // 解决Vulkan NDC坐标系差异
        // Vulkan NDC坐标Y朝下 而GL D3D都是朝上
        CreateInfo.height = -Viewport.Size.Y();
        CreateInfo.minDepth = Viewport.MinDepth;
        CreateInfo.maxDepth = Viewport.MaxDepth;
        return CreateInfo;
      }) |
      Ranges::To<Array<VkViewport>>();
  auto Scissors = //
      Desc.Viewport.Scissors | Ranges::Views::Transform([](const RHIPipelineScissor& Scissor) {
        VkRect2D CreateInfo = {};
        CreateInfo.offset.x = Scissor.Offset.X();
        CreateInfo.offset.y = Scissor.Offset.Y();
        CreateInfo.extent.width = Scissor.Extent.X();
        CreateInfo.extent.height = Scissor.Extent.Y();
        return CreateInfo;
      }) |
      Ranges::To<Array<VkRect2D>>();
  ViewportState.viewportCount = Viewports.Count();
  ViewportState.pViewports = Viewports.Data();
  ViewportState.scissorCount = Scissors.Count();
  ViewportState.pScissors = Scissors.Data();
  PipelineCreateInfo.pViewportState = &ViewportState;

  VkPipelineRasterizationStateCreateInfo RasterizationState = {};
  RasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  RasterizationState.cullMode = RHICullModeToVkCullMode(Desc.Rasterization.CullMode);
  RasterizationState.frontFace = RHIFrontFaceToVkFrontFace(Desc.Rasterization.FrontFace);
  RasterizationState.depthClampEnable = Desc.Rasterization.EnableDepthClamp;
  RasterizationState.rasterizerDiscardEnable = Desc.Rasterization.EnableDiscard;
  RasterizationState.polygonMode = RHIPolygonModeToVkPolygonMode(Desc.Rasterization.PolygonMode);
  RasterizationState.lineWidth = Desc.Rasterization.LineWidth;
  RasterizationState.depthBiasEnable = Desc.Rasterization.EnableDepthBias;
  PipelineCreateInfo.pRasterizationState = &RasterizationState;

  VkPipelineMultisampleStateCreateInfo MultisampleState = {};
  MultisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  MultisampleState.rasterizationSamples = RHISampleCountToVkSampleCount(Desc.Multisample.RasterizationSamples);
  MultisampleState.sampleShadingEnable = Desc.Multisample.EnableSampleShading;
  PipelineCreateInfo.pMultisampleState = &MultisampleState;

  VkPipelineDepthStencilStateCreateInfo DepthStencilState = {};
  DepthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  DepthStencilState.depthTestEnable = Desc.DepthStencil.EnableDepthTest;
  DepthStencilState.depthWriteEnable = Desc.DepthStencil.EnableDepthWrite;
  DepthStencilState.depthCompareOp = RHICompareOpToVkCompareOp(Desc.DepthStencil.DepthCompareOp);
  DepthStencilState.depthBoundsTestEnable = Desc.DepthStencil.EnableDepthBounds;
  DepthStencilState.stencilTestEnable = Desc.DepthStencil.EnableStencilTest;
  PipelineCreateInfo.pDepthStencilState = &DepthStencilState;

  VkPipelineColorBlendStateCreateInfo ColorBlendState = {};
  ColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  ColorBlendState.logicOpEnable = Desc.ColorBlend.EnableLogicOp;
  ColorBlendState.logicOp = RHILogicOpToVkLogicOp(Desc.ColorBlend.LogicOp);
  std::ranges::copy(Desc.ColorBlend.BlendConstants, ColorBlendState.blendConstants);
  auto ColorBlendAttachments = //
      Desc.ColorBlend.Attachments | Ranges::Views::Transform([](const RHIPipelineColorBlendAttachmentDesc& BlendDesc) {
        VkPipelineColorBlendAttachmentState Attachment = {};
        Attachment.alphaBlendOp = RHIBlendOpToVkBlendOp(BlendDesc.AlphaBlendOp);
        Attachment.blendEnable = BlendDesc.EnableBlend;
        Attachment.colorBlendOp = RHIBlendOpToVkBlendOp(BlendDesc.ColorBlendOp);
        Attachment.colorWriteMask = RHIColorComponentToVkColorComponent(BlendDesc.ColorWriteMask);
        Attachment.dstAlphaBlendFactor = RHIBlendFactorToVkBlendFactor(BlendDesc.DstAlphaBlendFactor);
        Attachment.dstColorBlendFactor = RHIBlendFactorToVkBlendFactor(BlendDesc.DstColorBlendFactor);
        Attachment.srcAlphaBlendFactor = RHIBlendFactorToVkBlendFactor(BlendDesc.SrcAlphaBlendFactor);
        Attachment.srcColorBlendFactor = RHIBlendFactorToVkBlendFactor(BlendDesc.SrcColorBlendFactor);
        return Attachment;
      }) |
      Ranges::To<Array<VkPipelineColorBlendAttachmentState>>();
  ColorBlendState.attachmentCount = static_cast<uint32_t>(ColorBlendAttachments.Count());
  ColorBlendState.pAttachments = ColorBlendAttachments.Data();
  PipelineCreateInfo.pColorBlendState = &ColorBlendState;

  PipelineCreateInfo.renderPass = Desc.RenderPass->GetNativeHandleT<VkRenderPass>();
  PipelineCreateInfo.subpass = Desc.Subpass;
  PipelineCreateInfo.layout = Desc.Layout->GetNativeHandleT<VkPipelineLayout>();
  if (VkResult Result = vkCreateGraphicsPipelines(GetVulkanGfxContex()->GetDevice(), VK_NULL_HANDLE, 1, &PipelineCreateInfo, nullptr, &mPipeline); Result != VK_SUCCESS) {
    LOG_ERROR_TAG("RHI.Vulkan", "创建图形管线失败, Code={}", Result);
    mPipeline = VK_NULL_HANDLE;
  }
}

RHIPipeline_Vulkan::~RHIPipeline_Vulkan() {
  if (mPipeline) {
    vkDestroyPipeline(GetVulkanGfxContexRef().GetDevice(), mPipeline, nullptr);
    mPipeline = nullptr;
  }
}