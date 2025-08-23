#pragma once
#include "IRHIResource.h"
#include "Math/Vector.h"
#include "RHIFormat.h"

struct RHIPipelineLayoutDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<class RHIDescriptorSetLayout*>, Layouts, {})
};

class RHIPipelineLayout : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::PipelineLayout; }
};

struct PipelineShaderStageDesc {
  RHI_DEFINE_BUILDER_FIELD_PTR(class ERHIShaderModule*, SShaderModule, nullptr);
  RHI_DEFINE_BUILDER_FIELD_PTR(const char*, EntryName, "main");
  RHI_DEFINE_BUILDER_FIELD(ERHIShaderStageBits, ShaderStage, ERHIShaderStageBits::SSB_MAX);
  // TODO: 特化信息
};

struct VertexInputBindingDesc {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Binding, 0);                                         // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, Stride, 0);                                          // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIVertexInputRate, InputRate, ERHIVertexInputRate::Count); // NECESSARY
};

struct VertexInputAttributeDesc {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Location, 0)                   // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, Binding, 0)                    // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIFormat, Format, ERHIFormat::Count) // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, Offset, 0)
};

struct PipelineVertexInputDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<VertexInputBindingDesc>, VertexBindings, {});
  RHI_DEFINE_BUILDER_FIELD(Array<VertexInputAttributeDesc>, VertexAttributes, {});
};

struct PipelineInputAssemblyDesc {
  RHI_DEFINE_BUILDER_FIELD(ERHIPrimitiveTopology, Topology, ERHIPrimitiveTopology::TriangleList)
};

struct PipelineViewport {
  RHI_DEFINE_BUILDER_FIELD(Vector2f, Position, Vector2f(0.f, 0.f)) // 左上角
  RHI_DEFINE_BUILDER_FIELD(Vector2f, Size, Vector2f(0.f, 0.f))
  RHI_DEFINE_BUILDER_FIELD(float, MinDepth, 0)
  RHI_DEFINE_BUILDER_FIELD(float, MaxDepth, 1)
};

struct PipelineScissor {
  RHI_DEFINE_BUILDER_FIELD(Vector2i, Offset, Vector2i(0, 0))
  RHI_DEFINE_BUILDER_FIELD(Vector2i, Extent, Vector2i(0, 0))
};

struct PipelineViewportDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<PipelineViewport>, Viewports, {})
  RHI_DEFINE_BUILDER_FIELD(Array<PipelineScissor>, Scissors, {})
};

struct PipelineRasterizationDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthClamp, false)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDiscard, false)
  RHI_DEFINE_BUILDER_FIELD(ERHICullMode, CullMode, ERHICullMode::Back)
  RHI_DEFINE_BUILDER_FIELD(ERHIFrontFace, FrontFace, ERHIFrontFace::Clockwise)
  RHI_DEFINE_BUILDER_FIELD(ERHIPolygonMode, PolygonMode, ERHIPolygonMode::Fill)
  RHI_DEFINE_BUILDER_FIELD(float, LineWidth, 1.f)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthBias, false)
};

struct PipelineMultisampleDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableSampleShading, false)
  RHI_DEFINE_BUILDER_FIELD(ERHISampleCount, RasterizationSamples, ERHISampleCount::SC_1)
};

struct PipelineDepthStencilDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthTest, false)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthWrite, false)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableStencilTest, false)
  RHI_DEFINE_BUILDER_FIELD(bool, EnableDepthBounds, false)
  RHI_DEFINE_BUILDER_FIELD(ERHICompareOp, DepthCompareOp, ERHICompareOp::Less)
};

struct PipelineColorBlendAttachmentDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableBlend, false)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendFactor, SrcColorBlendFactor, ERHIBlendFactor::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendFactor, DstColorBlendFactor, ERHIBlendFactor::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendOp, ColorBlendOp, ERHIBlendOp::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendFactor, SrcAlphaBlendFactor, ERHIBlendFactor::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendFactor, DstAlphaBlendFactor, ERHIBlendFactor::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIBlendOp, AlphaBlendOp, ERHIBlendOp::Count)
  RHI_DEFINE_BUILDER_FIELD(ERHIColorComponent, ColorWriteMask, ECCB_R | ECCB_G | ECCB_B | ECCB_A)
};

struct PipelineColorBlendDesc {
  RHI_DEFINE_BUILDER_FIELD(bool, EnableLogicOp, false)
  RHI_DEFINE_BUILDER_FIELD(ERHILogicOp, LogicOp, ERHILogicOp::Copy)
  RHI_DEFINE_BUILDER_FIELD(Array<PipelineColorBlendAttachmentDesc>, Attachments, {});
  using BlendConstantsType = std::array<float, 4>;
  BlendConstantsType BlendConstants = {1.f, 1, 1, 1};
  [[nodiscard]] FORCE_INLINE BlendConstantsType GetBlendConstants() const { return this->BlendConstants; }
  FORCE_INLINE auto& SetBlendConstants(const BlendConstantsType& Value) {
    this->BlendConstants = Value;
    return *this;
  }
};

struct GraphicsPipelineDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<PipelineShaderStageDesc>, ShaderStages, {})
  RHI_DEFINE_BUILDER_FIELD(PipelineVertexInputDesc, VertexInput, {})
  RHI_DEFINE_BUILDER_FIELD(PipelineInputAssemblyDesc, InputAssembly, {})
  RHI_DEFINE_BUILDER_FIELD(PipelineViewportDesc, Viewport, {})
  RHI_DEFINE_BUILDER_FIELD(PipelineRasterizationDesc, Rasterization, {})
  RHI_DEFINE_BUILDER_FIELD(PipelineMultisampleDesc, Multisample, {})
  RHI_DEFINE_BUILDER_FIELD(PipelineDepthStencilDesc, DepthStencil, {})
  RHI_DEFINE_BUILDER_FIELD(PipelineColorBlendDesc, ColorBlend, {})
  RHI_DEFINE_BUILDER_FIELD_PTR(class RHIRenderPass*, RenderPass, nullptr)
  RHI_DEFINE_BUILDER_FIELD(UInt32, Subpass, 0)
  RHI_DEFINE_BUILDER_FIELD_PTR(class RHIPipelineLayout*, Layout, nullptr)
};

class RHIPipeline : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::Pipeline; }
};
