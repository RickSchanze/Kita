//
// Created by kita on 25-8-26.
//

#include "RenderPass_Vulkan.h"

#include "GfxContext_Vulkan.h"
#include "RHIEnums_Vulkan.h"

RHIRenderPass_Vulkan::RHIRenderPass_Vulkan(const RHIRenderPassDesc& Desc) {
  auto& Ctx = GetVulkanGfxContexRef();
  VkRenderPassCreateInfo RenderPassInfo{};

  Array<VkAttachmentDescription> Attachments;
  Array<VkSubpassDescription> Subpasses;
  Array<VkSubpassDependency> Dependencies;

  // Attachments
  for (const auto& Attachment : Desc.Attachments) {
    VkAttachmentDescription AttachmentDesc{};
    AttachmentDesc.format = RHIFormatToVkFormat(Attachment.Format);
    AttachmentDesc.samples = RHISampleCountToVkSampleCount(Attachment.Samples);
    AttachmentDesc.loadOp = RHIAttachmentLoadOpToVkAttachmentLoadOp(Attachment.LoadOp);
    AttachmentDesc.storeOp = RHIAttachmentStoreOpToVkAttachmentStoreOp(Attachment.StoreOp);
    AttachmentDesc.stencilLoadOp = RHIAttachmentLoadOpToVkAttachmentLoadOp(Attachment.StencilLoadOp);
    AttachmentDesc.stencilStoreOp = RHIAttachmentStoreOpToVkAttachmentStoreOp(Attachment.StencilStoreOp);
    AttachmentDesc.initialLayout = RHIImageLayoutToVkImageLayout(Attachment.InitialLayout);
    AttachmentDesc.finalLayout = RHIImageLayoutToVkImageLayout(Attachment.FinalLayout);
  }
  RenderPassInfo.attachmentCount = Attachments.Count();
  RenderPassInfo.pAttachments = Attachments.Data();

  // Subpasses
  {
    Array<Array<VkAttachmentReference>> RenderPassInputAttachments;
    Array<Array<VkAttachmentReference>> RenderPassColorAttachments;

    Array<Optional<VkAttachmentReference>> ResolveAttachments;
    Array<Optional<VkAttachmentReference>> DepthStencilAttachments;

    for (const auto& Subpass : Desc.Subpasses) {
      Array<VkAttachmentReference> InputAttachments;
      for (const auto& InputAttachment : Subpass.InputAttachments) {
        VkAttachmentReference AttachmentRef{};
        AttachmentRef.attachment = InputAttachment.Attachment;
        AttachmentRef.layout = RHIImageLayoutToVkImageLayout(InputAttachment.Layout);
        InputAttachments.Add(AttachmentRef);
      }
      RenderPassInputAttachments.Add(InputAttachments);
      Array<VkAttachmentReference> ColorAttachments;
      for (const auto& ColorAttachment : Subpass.ColorAttachments) {
        VkAttachmentReference AttachmentRef{};
        AttachmentRef.attachment = ColorAttachment.Attachment;
        AttachmentRef.layout = RHIImageLayoutToVkImageLayout(ColorAttachment.Layout);
        ColorAttachments.Add(AttachmentRef);
      }
      RenderPassColorAttachments.Add(ColorAttachments);
      if (Subpass.ResolveAttachment) {
        VkAttachmentReference AttachmentRef{};
        AttachmentRef.attachment = Subpass.ResolveAttachment->Attachment;
        AttachmentRef.layout = RHIImageLayoutToVkImageLayout(Subpass.ResolveAttachment->Layout);
        ResolveAttachments.Add(AttachmentRef);
      }
      if (Subpass.DepthStencilAttachment) {
        VkAttachmentReference AttachmentRef{};
        AttachmentRef.attachment = Subpass.DepthStencilAttachment->Attachment;
        AttachmentRef.layout = RHIImageLayoutToVkImageLayout(Subpass.DepthStencilAttachment->Layout);
        DepthStencilAttachments.Add(AttachmentRef);
      }
      VkSubpassDescription SubpassDesc{};
      SubpassDesc.colorAttachmentCount = ColorAttachments.Count();
      SubpassDesc.pColorAttachments = RenderPassColorAttachments.Last().Data();
      SubpassDesc.inputAttachmentCount = InputAttachments.Count();
      SubpassDesc.pInputAttachments = RenderPassInputAttachments.Last().Data();
      if (Subpass.ResolveAttachment) {
        SubpassDesc.pResolveAttachments = &ResolveAttachments.Last().Value();
      }
      if (Subpass.DepthStencilAttachment) {
        SubpassDesc.pDepthStencilAttachment = &DepthStencilAttachments.Last().Value();
      }
      Subpasses.Add(SubpassDesc);
    }
    RenderPassInfo.subpassCount = Subpasses.Count();
    RenderPassInfo.pSubpasses = Subpasses.Data();
  }
  // Dependencies
  {
    for (const auto& Dependency : Desc.Dependencies) {
      VkSubpassDependency DependencyDesc{};
      DependencyDesc.srcSubpass = Dependency.SrcSubpass;
      DependencyDesc.dstSubpass = Dependency.DstSubpass;
      DependencyDesc.srcStageMask = RHIShaderStageToVkShaderStage(Dependency.SrcStageMask);
      DependencyDesc.dstStageMask = RHIShaderStageToVkShaderStage(Dependency.DstStageMask);
      DependencyDesc.srcAccessMask = RHIAccessFlagToVkAccessFlag(Dependency.SrcAccessMask);
      DependencyDesc.dstAccessMask = RHIAccessFlagToVkAccessFlag(Dependency.DstAccessMask);
      Dependencies.Add(DependencyDesc);
    }
    RenderPassInfo.dependencyCount = Dependencies.Count();
    RenderPassInfo.pDependencies = Dependencies.Data();
  }
  if (VkResult Result = vkCreateRenderPass(Ctx.GetDevice(), &RenderPassInfo, nullptr, &mRenderPass); Result != VK_SUCCESS) {
    gLogger.Error("RHI.Vulkan", "创建RenderPass失败, Code={}", Result);
    mRenderPass = nullptr;
  }
}

RHIRenderPass_Vulkan::~RHIRenderPass_Vulkan() {
  if (mRenderPass) {
    auto& Ctx = GetVulkanGfxContexRef();
    vkDestroyRenderPass(Ctx.GetDevice(), mRenderPass, nullptr);
    mRenderPass = nullptr;
  }
}