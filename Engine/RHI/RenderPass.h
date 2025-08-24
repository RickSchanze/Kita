#pragma once
#include "IRHIResource.h"
#include "RHIEnums.h"

struct RHISubpassDependency {
  RHI_DEFINE_BUILDER_FIELD(UInt32, SrcSubpass, 0);
  RHI_DEFINE_BUILDER_FIELD(UInt32, DstSubpass, 0);
  RHI_DEFINE_BUILDER_FIELD(ERHIPipelineStageFlagBits, SrcStageMask, ERHIPipelineStageFlagBits::PSFB_Max); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIPipelineStageFlagBits, DstStageMask, ERHIPipelineStageFlagBits::PSFB_Max); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIAccessFlagBits, SrcAccessMask, ERHIAccessFlagBits::AFB_Max);               // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIAccessFlagBits, DstAccessMask, ERHIAccessFlagBits::AFB_Max);
};

struct RHIAttachmentRef {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Attachment, 0);                               // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIImageLayout, Layout, ERHIImageLayout::Undefined); // NECESSARY
};

struct RHISubpassDesc {
  RHI_DEFINE_BUILDER_FIELD(ERHIPipelineBindPoint, PipelineBindPoint, ERHIPipelineBindPoint::Graphics);
  RHI_DEFINE_BUILDER_FIELD(Array<RHIAttachmentRef>, InputAttachments, {});
  RHI_DEFINE_BUILDER_FIELD(Array<RHIAttachmentRef>, ColorAttachments, {});
  RHI_DEFINE_BUILDER_FIELD(Optional<RHIAttachmentRef>, DepthStencilAttachment, {});
  RHI_DEFINE_BUILDER_FIELD(Optional<RHIAttachmentRef>, ResolveAttachment, {});
};

struct RHIAttachmentDesc {
  RHI_DEFINE_BUILDER_FIELD(ERHIFormat, Format, ERHIFormat::Count); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHISampleCount, Samples, ERHISampleCount::SC_1);
  RHI_DEFINE_BUILDER_FIELD(ERHIAttachmentLoadOperation, LoadOp, ERHIAttachmentLoadOperation::Count); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIAttachmentStoreOperation, StoreOp, ERHIAttachmentStoreOperation::Count);
  RHI_DEFINE_BUILDER_FIELD(ERHIAttachmentLoadOperation, StencilLoadOp, ERHIAttachmentLoadOperation::DontCare);
  RHI_DEFINE_BUILDER_FIELD(ERHIAttachmentStoreOperation, StencilStoreOp, ERHIAttachmentStoreOperation::DontCare);
  RHI_DEFINE_BUILDER_FIELD(ERHIImageLayout, InitialLayout, ERHIImageLayout::Undefined); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIImageLayout, FinalLayout, ERHIImageLayout::Undefined);   // NECESSARY
};

struct RHIRenderPassDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<RHISubpassDesc>, Subpasses, {});
  RHI_DEFINE_BUILDER_FIELD(Array<RHIAttachmentDesc>, Attachments, {});
  RHI_DEFINE_BUILDER_FIELD(Array<RHISubpassDependency>, Dependencies, {});
};

class RHIRenderPass : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::RenderPass; }
};
