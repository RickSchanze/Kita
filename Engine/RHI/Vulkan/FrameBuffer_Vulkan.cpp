//
// Created by kita on 25-8-24.
//

#include "FrameBuffer_Vulkan.h"

#include "GfxContext_Vulkan.h"
#include "RHI/ImageView.h"
#include "RHI/RenderPass.h"

#include <vulkan/vulkan_core.h>

RHIFrameBuffer_Vulkan::~RHIFrameBuffer_Vulkan() {
  if (mFramebuffer) {
    vkDestroyFramebuffer(GetVulkanGfxContexRef().GetDevice(), mFramebuffer, nullptr);
    mFramebuffer = nullptr;
  }
}

RHIFrameBuffer_Vulkan::RHIFrameBuffer_Vulkan(const RHIFrameBufferDesc& Desc) {
  VkFramebufferCreateInfo FramebufferInfo = {};
  FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  FramebufferInfo.renderPass = Desc.RenderPass->GetNativeHandleT<VkRenderPass>();
  Array<VkImageView> ImageViews{};
  ImageViews.Reserve(Desc.Attachments.Count());
  for (auto& Attachment : Desc.Attachments) {
    ImageViews.Add(Attachment->GetNativeHandleT<VkImageView>());
  }
  FramebufferInfo.attachmentCount = ImageViews.Count();
  FramebufferInfo.pAttachments = ImageViews.Data();
  FramebufferInfo.width = Desc.Width;
  FramebufferInfo.height = Desc.Height;
  FramebufferInfo.layers = Desc.Layers;
  if (VkResult Result = vkCreateFramebuffer(GetVulkanGfxContexRef().GetDevice(), &FramebufferInfo, nullptr, &mFramebuffer); Result != VK_SUCCESS) {
    gLogger.Error("RHI", "创建FrameBuffer失败, 错误码={}", Result);
  }
  mRenderPass = Desc.RenderPass;
#if KITA_DEBUG_NAME
  GetVulkanGfxContexRef().SetDebugName(Format("FrameBuffer_{}", Desc.DebugName), VK_OBJECT_TYPE_FRAMEBUFFER, reinterpret_cast<UInt64>(mFramebuffer));
#endif
}