#pragma once
#include "Image_Vulkan.h"
#include "RHI/ImageView.h"

class RHIImageView_Vulkan : public RHIImageView {
public:
  // 当传入图像为交换链图像时用这个
  RHIImageView_Vulkan(VkImage SwapchainImage, VkFormat Format);
  explicit RHIImageView_Vulkan(const RHIImageViewDesc& Desc);

  virtual ~RHIImageView_Vulkan() override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mImageView; }

private:
  VkImageView mImageView = VK_NULL_HANDLE;
};
