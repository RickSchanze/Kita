//
// Created by kita on 25-8-24.
//

#include "ImageView_Vulkan.h"

#include "GfxContext_Vulkan.h"
#include "RHIEnums_Vulkan.h"

RHIImageView_Vulkan::RHIImageView_Vulkan(VkImage SwapchainImage, VkFormat Format) {
  VkImageViewCreateInfo ImageViewInfo{};
  ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ImageViewInfo.image = SwapchainImage;
  ImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  ImageViewInfo.format = Format;
  ImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  ImageViewInfo.subresourceRange.baseMipLevel = 0;
  ImageViewInfo.subresourceRange.levelCount = 1;
  ImageViewInfo.subresourceRange.baseArrayLayer = 0;
  ImageViewInfo.subresourceRange.layerCount = 1;

  if (const VkResult Result = vkCreateImageView(GetVulkanGfxContexRef().GetDevice(), &ImageViewInfo, nullptr, &mImageView); Result != VK_SUCCESS) {
    gLogger.Error("RHI", "创建交换链图像视图失败, 错误码={}", static_cast<Int32>(Result));
  }
  mRelatedImage = nullptr;
}

RHIImageView_Vulkan::RHIImageView_Vulkan(const RHIImageViewDesc& Desc) {
  ASSERT_MSG(Desc.SourceImage, "用于创建ImageView的Image必修有效");
  VkImageViewCreateInfo ImageViewInfo{};
  ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ImageViewInfo.image = Desc.SourceImage->GetNativeHandleT<VkImage>();
  ImageViewInfo.viewType = RHIImageDimensionToVkImageViewType(Desc.ViewType);
  ImageViewInfo.format = RHIFormatToVkFormat(Desc.Format == ERHIFormat::Count ? Desc.SourceImage->GetDesc().Format : Desc.Format);
  ImageViewInfo.subresourceRange.aspectMask = RHIImageAspectToVkImageAspect(Desc.SubresourceRange.AspectMask);
  ImageViewInfo.subresourceRange.baseMipLevel = Desc.SubresourceRange.BaseMipLevel;
  ImageViewInfo.subresourceRange.levelCount = Desc.SubresourceRange.LevelCount;
  ImageViewInfo.subresourceRange.baseArrayLayer = Desc.SubresourceRange.BaseArrayLayer;
  ImageViewInfo.subresourceRange.layerCount = Desc.SubresourceRange.LayerCount;
  ImageViewInfo.components.r = RHIComponentMappingElementToVkComponentSwizzle(Desc.ComponentMapping.R);
  ImageViewInfo.components.g = RHIComponentMappingElementToVkComponentSwizzle(Desc.ComponentMapping.G);
  ImageViewInfo.components.b = RHIComponentMappingElementToVkComponentSwizzle(Desc.ComponentMapping.B);
  ImageViewInfo.components.a = RHIComponentMappingElementToVkComponentSwizzle(Desc.ComponentMapping.A);
  if (const VkResult Result = vkCreateImageView(GetVulkanGfxContexRef().GetDevice(), &ImageViewInfo, nullptr, &mImageView); Result != VK_SUCCESS) {
    gLogger.Error("RHI", "创建图像视图失败, 错误码={}", Result);
  }
  mRelatedImage = Desc.SourceImage;
}

RHIImageView_Vulkan::~RHIImageView_Vulkan() {
  mRelatedImage = nullptr;
  if (mImageView) {
    vkDestroyImageView(GetVulkanGfxContexRef().GetDevice(), mImageView, nullptr);
    mImageView = nullptr;
  }
}