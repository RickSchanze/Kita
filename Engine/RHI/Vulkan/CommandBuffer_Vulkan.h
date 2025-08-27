#pragma once
#include "RHI/CommandBuffer.h"

#include <vulkan/vulkan_core.h>

class RHICommandPool_Vulkan : public RHICommandPool {
public:
  explicit RHICommandPool_Vulkan(ERHIQueueFamilyType Family);
  virtual ~RHICommandPool_Vulkan() override;

  virtual void* GetNativeHandle() const override { return mPool; }

  virtual UniquePtr<RHICommandBuffer> CreateCommandBuffer() override;

  virtual void Reset() override;

private:
  VkCommandPool mPool = VK_NULL_HANDLE;
  ERHIQueueFamilyType mFamily;
};

class RHICommandBuffer_Vulkan : public RHICommandBuffer {
  friend class RHICommandPool_Vulkan;

public:
  virtual void* GetNativeHandle() const override final { return mBuffer; }

  virtual TaskHandle Execute(StringView DebugName) override;

private:
  VkCommandBuffer mBuffer = VK_NULL_HANDLE;
  RHICommandPool_Vulkan* mParentPool = nullptr;
};
