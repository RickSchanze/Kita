#pragma once
#include "CommandBuffer_Vulkan.h"
#include "RHI/Buffer.h"

class RHIBuffer_Vulkan : public RHIBuffer {
public:
  explicit RHIBuffer_Vulkan(const RHIBufferDesc& Desc);
  virtual ~RHIBuffer_Vulkan() override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mBuffer; }

  virtual void* Map(UInt64 Size, UInt64 Offset) override;
  virtual void Unmap(void* MappedPtr) override;

private:
  VkBuffer mBuffer = VK_NULL_HANDLE;
  VkDeviceMemory mBufferMemory = VK_NULL_HANDLE;
};
