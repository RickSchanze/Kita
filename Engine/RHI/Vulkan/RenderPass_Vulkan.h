#pragma once
#include "RHI/RenderPass.h"

#include <vulkan/vulkan_core.h>

class RHIRenderPass_Vulkan : public RHIRenderPass {
public:
  explicit RHIRenderPass_Vulkan(VkRenderPass RenderPass) : mRenderPass(RenderPass) {}
  explicit RHIRenderPass_Vulkan(const RHIRenderPassDesc& Desc);
  virtual ~RHIRenderPass_Vulkan() override;
  [[nodiscard]] virtual void* GetNativeHandle() const override final { return mRenderPass; }

private:
  VkRenderPass mRenderPass = VK_NULL_HANDLE;
};