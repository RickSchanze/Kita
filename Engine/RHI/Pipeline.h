#pragma once
#include "IRHIResource.h"
#include "Math/Vector.h"
#include "RHIEnums.h"

struct RHIPipelineLayoutDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<class RHIDescriptorSetLayout*>, Layouts, {})
};

class RHIPipelineLayout : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::PipelineLayout; }
};

struct RHIPipelineShaderStageDesc {
  RHI_DEFINE_BUILDER_FIELD_PTR(class RHIShaderModule*, ShaderModule, nullptr);
  RHI_DEFINE_BUILDER_FIELD_PTR(const char*, EntryName, "main");
  RHI_DEFINE_BUILDER_FIELD(ERHIShaderStageBits, ShaderStage, ERHIShaderStageBits::SSB_MAX);
  // TODO: 特化信息
};

struct RHIVertexInputBindingDesc {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Binding, 0);                                         // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, Stride, 0);                                          // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIVertexInputRate, InputRate, ERHIVertexInputRate::Count); // NECESSARY
};

struct RHIVertexInputAttributeDesc {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Location, 0)                   // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, Binding, 0)                    // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIFormat, Format, ERHIFormat::Count) // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, Offset, 0)
};

struct RHIPipelineVertexInputDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<RHIVertexInputBindingDesc>, VertexBindings, {});
  RHI_DEFINE_BUILDER_FIELD(Array<RHIVertexInputAttributeDesc>, VertexAttributes, {});
};

struct RHIPipelineInputAssemblyDesc {
  RHI_DEFINE_BUILDER_FIELD(ERHIPrimitiveTopology, Topology, ERHIPrimitiveTopology::TriangleList)
};

struct RHIPipelineViewport {
  RHI_DEFINE_BUILDER_FIELD(Vector2f, Position, Vector2f(0.f, 0.f)) // 左上角
  RHI_DEFINE_BUILDER_FIELD(Vector2f, Size, Vector2f(0.f, 0.f))
  RHI_DEFINE_BUILDER_FIELD(float, MinDepth, 0)
  RHI_DEFINE_BUILDER_FIELD(float, MaxDepth, 1)
};

struct RHIPipelineScissor {
  RHI_DEFINE_BUILDER_FIELD(Vector2i, Offset, Vector2i(0, 0))
  RHI_DEFINE_BUILDER_FIELD(Vector2i, Extent, Vector2i(0, 0))
};

struct RHIPipelineViewportDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<RHIPipelineViewport>, Viewports, {})
  RHI_DEFINE_BUILDER_FIELD(Array<RHIPipelineScissor>, Scissors, {})
};

struct RHIPipelineRasterizationDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthClamp, false)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDiscard, false)
  RHI_DEFINE_BUILDER_FIELD(ERHICullMode, CullMode, ERHICullMode::Back)
  RHI_DEFINE_BUILDER_FIELD(ERHIFrontFace, FrontFace, ERHIFrontFace::Clockwise)
  RHI_DEFINE_BUILDER_FIELD(ERHIPolygonMode, PolygonMode, ERHIPolygonMode::Fill)
  RHI_DEFINE_BUILDER_FIELD(float, LineWidth, 1.f)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthBias, false)
};

struct RHIPipelineMultisampleDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableSampleShading, false)
  RHI_DEFINE_BUILDER_FIELD(ERHISampleCount, RasterizationSamples, ERHISampleCount::SC_1)
};

struct RHIPipelineDepthStencilDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthTest, false)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthWrite, false)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableStencilTest, false)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthBounds, false)
  RHI_DEFINE_BUILDER_FIELD(ERHICompareOp, DepthCompareOp, ERHICompareOp::Less)
};

struct RHIPipelineColorBlendAttachmentDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableBlend, false)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendFactor, SrcColorBlendFactor, ERHIBlendFactor::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendFactor, DstColorBlendFactor, ERHIBlendFactor::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendOp, ColorBlendOp, ERHIBlendOp::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendFactor, SrcAlphaBlendFactor, ERHIBlendFactor::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendFactor, DstAlphaBlendFactor, ERHIBlendFactor::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendOp, AlphaBlendOp, ERHIBlendOp::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIColorComponent, ColorWriteMask, ECCB_R | ECCB_G | ECCB_B | ECCB_A)
};

struct RHIPipelineColorBlendDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableLogicOp, false)
  RHI_DEFINE_BUILDER_FIELD(ERHILogicOp, LogicOp, ERHILogicOp::Copy)
  RHI_DEFINE_BUILDER_FIELD(Array<RHIPipelineColorBlendAttachmentDesc>, Attachments, {});
  using BlendConstantsType = std::array<float, 4>;
  BlendConstantsType BlendConstants = {1.f, 1, 1, 1};
  [[nodiscard]] FORCE_INLINE BlendConstantsType GetBlendConstants() const { return this->BlendConstants; }
  FORCE_INLINE auto& SetBlendConstants(const BlendConstantsType& Value) {
    this->BlendConstants = Value;
    return *this;
  }
};

struct RHIGraphicsPipelineDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<RHIPipelineShaderStageDesc>, ShaderStages, {})
  RHI_DEFINE_BUILDER_FIELD(RHIPipelineVertexInputDesc, VertexInput, {})
  RHI_DEFINE_BUILDER_FIELD(RHIPipelineInputAssemblyDesc, InputAssembly, {})
  RHI_DEFINE_BUILDER_FIELD(RHIPipelineViewportDesc, Viewport, {})
  RHI_DEFINE_BUILDER_FIELD(RHIPipelineRasterizationDesc, Rasterization, {})
  RHI_DEFINE_BUILDER_FIELD(RHIPipelineMultisampleDesc, Multisample, {})
  RHI_DEFINE_BUILDER_FIELD(RHIPipelineDepthStencilDesc, DepthStencil, {})
  RHI_DEFINE_BUILDER_FIELD(RHIPipelineColorBlendDesc, ColorBlend, {})
  RHI_DEFINE_BUILDER_FIELD_PTR(class RHIRenderPass*, RenderPass, nullptr)
  RHI_DEFINE_BUILDER_FIELD(UInt32, Subpass, 0)
  RHI_DEFINE_BUILDER_FIELD_PTR(class RHIPipelineLayout*, Layout, nullptr)
};

class RHIPipeline : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::Pipeline; }
};
