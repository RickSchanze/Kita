#pragma once
#include "RHI/FrameBuffer.h"
#include "vulkan/vulkan.h"

class RHIFrameBuffer_Vulkan : public RHIFrameBuffer {
public:
  [[nodiscard]] virtual void* GetNativeHandle() const override { return mFramebuffer; }

  explicit RHIFrameBuffer_Vulkan(const RHIFrameBufferDesc& Desc);
  virtual ~RHIFrameBuffer_Vulkan() override;

private:
  VkFramebuffer mFramebuffer = VK_NULL_HANDLE;
};
