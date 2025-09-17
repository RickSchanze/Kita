#pragma once
#include "RHI/Image.h"

#include <vulkan/vulkan_core.h>

class RHIImage_Vulkan final : public RHIImage {
public:
  explicit RHIImage_Vulkan(const RHIImageDesc& Desc);
  virtual ~RHIImage_Vulkan() override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mNativeImage; }

protected:
  VkImage mNativeImage = VK_NULL_HANDLE;
  VkDeviceMemory mMemory = VK_NULL_HANDLE;
};
