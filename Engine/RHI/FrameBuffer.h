#pragma once
#include "IRHIResource.h"
#include "RHIFormat.h"

class RHIRenderPass;
class RHIImageView;
struct FrameBufferDesc {
  RHI_DEFINE_BUILDER_FIELD_PTR(RHIRenderPass*, RenderPass, nullptr); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, Layers, 1);                       // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, Width, 0);                        // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(UInt32, Height, 0);                       // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(Array<RHIImageView*>, Attachments, {});   // NECESSARY
};

class RHIFrameBuffer : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::FrameBuffer; }
};