#pragma once
#include "Core/HashUtils.h"
#include "IRHIResource.h"
#include "RHIEnums.h"

struct RHISubpassDependency {
  RHI_DEFINE_BUILDER_FIELD(UInt32, SrcSubpass, 0);
  RHI_DEFINE_BUILDER_FIELD(UInt32, DstSubpass, 0);
  RHI_DEFINE_BUILDER_FIELD(ERHIPipelineStageFlagBits, SrcStageMask, ERHIPipelineStageFlagBits::PSFB_Max); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIPipelineStageFlagBits, DstStageMask, ERHIPipelineStageFlagBits::PSFB_Max); // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIAccessFlagBits, SrcAccessMask, ERHIAccessFlagBits::AFB_Max);               // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIAccessFlagBits, DstAccessMask, ERHIAccessFlagBits::AFB_Max);

  [[nodiscard]] UInt64 GetHashCode() const {
    UInt64 Hash = HashUtils::FNV_OFFSET_BASIS;
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(SrcSubpass));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(DstSubpass));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(SrcStageMask));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(DstStageMask));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(SrcAccessMask));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(DstAccessMask));
    return Hash;
  }
};

struct RHIAttachmentRef {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Attachment, 0);                               // NECESSARY
  RHI_DEFINE_BUILDER_FIELD(ERHIImageLayout, Layout, ERHIImageLayout::Undefined); // NECESSARY

  [[nodiscard]] UInt64 GetHashCode() const {
    UInt64 Hash = HashUtils::FNV_OFFSET_BASIS;
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(Attachment));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(Layout));
    return Hash;
  }
};

struct RHISubpassDesc {
  RHI_DEFINE_BUILDER_FIELD(ERHIPipelineBindPoint, PipelineBindPoint, ERHIPipelineBindPoint::Graphics);
  RHI_DEFINE_BUILDER_FIELD(Array<RHIAttachmentRef>, InputAttachments, {});
  RHI_DEFINE_BUILDER_FIELD(Array<RHIAttachmentRef>, ColorAttachments, {});
  RHI_DEFINE_BUILDER_FIELD(Optional<RHIAttachmentRef>, DepthStencilAttachment, {});
  RHI_DEFINE_BUILDER_FIELD(Optional<RHIAttachmentRef>, ResolveAttachment, {});

  [[nodiscard]] UInt64 GetHashCode() const {
    UInt64 Hash = HashUtils::FNV_OFFSET_BASIS;
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(PipelineBindPoint));
    Hash = HashUtils::HashCombine(Hash, ::GetHashCode(InputAttachments));
    Hash = HashUtils::HashCombine(Hash, ::GetHashCode(ColorAttachments));
    Hash = HashUtils::HashCombine(Hash, ::GetHashCode(DepthStencilAttachment));
    Hash = HashUtils::HashCombine(Hash, ::GetHashCode(ResolveAttachment));
    return Hash;
  }
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

  [[nodiscard]] UInt64 GetHashCode() const {
    UInt64 Hash = HashUtils::FNV_OFFSET_BASIS;
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(Format));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(Samples));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(LoadOp));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(StoreOp));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(StencilLoadOp));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(StencilStoreOp));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(InitialLayout));
    Hash = HashUtils::HashCombine(Hash, static_cast<UInt64>(FinalLayout));
    return Hash;
  }
};

struct RHIRenderPassDesc {
  RHI_DEFINE_BUILDER_FIELD(Array<RHISubpassDesc>, Subpasses, {});
  RHI_DEFINE_BUILDER_FIELD(Array<RHIAttachmentDesc>, Attachments, {});
  RHI_DEFINE_BUILDER_FIELD(Array<RHISubpassDependency>, Dependencies, {});

  [[nodiscard]] UInt64 GetHashCode() const {
    UInt64 Hash = HashUtils::FNV_OFFSET_BASIS;
    Hash = HashUtils::HashCombine(Hash, ::GetHashCode(Attachments));
    Hash = HashUtils::HashCombine(Hash, ::GetHashCode(Subpasses));
    Hash = HashUtils::HashCombine(Hash, ::GetHashCode(Dependencies));
    return Hash;
  }
};

class RHIRenderPass : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::RenderPass; }
};
