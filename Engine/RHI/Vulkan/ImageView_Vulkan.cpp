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
}

RHIImageView_Vulkan::RHIImageView_Vulkan(const RHIImageViewDesc& Desc) {
  ASSERT_MSG(Desc.SourceImage, "用于创建ImageView的Image必修有效");
  VkImageViewCreateInfo ImageViewInfo{};
  ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ImageViewInfo.image = Desc.SourceImage->GetNativeHandleT<VkImage>();
  ImageViewInfo.viewType = RHIImageDimensionToVkImageViewType(Desc.ViewType);
  ImageViewInfo.format = RHIFormatToVkFormat(Desc.Format == ERHIFormat::Count ? Desc.SourceImage->GetDesc().Format : Desc.Format);
  VkImageSubresourceRange Range{};
  if (Desc.SubresourceRange) {
    auto& MyRange = *Desc.SubresourceRange;
    Range.aspectMask = RHIImageAspectToVkImageAspect(MyRange.AspectMask);
    Range.baseMipLevel = MyRange.BaseMipLevel;
    Range.levelCount = MyRange.LevelCount;
    Range.baseArrayLayer = MyRange.BaseArrayLayer;
    Range.layerCount = MyRange.LayerCount;
  } else {
    auto& ImageDesc = Desc.SourceImage->GetDesc();
    if (True(ImageDesc.Usage & ERHIImageUsage::DepthStencil)) {
      Range.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    } else if (True(ImageDesc.Usage & ERHIImageUsage::ShaderRead)) {
      Range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }
    Range.levelCount = ImageDesc.MipLevels;
    Range.layerCount = ImageDesc.ArrayLayers;
    Range.baseArrayLayer = 0;
    Range.baseMipLevel = 0;
  }
  ImageViewInfo.subresourceRange = Range;
  VkComponentMapping Mapping;
  if (Desc.ComponentMapping) {
    Mapping.r = RHIComponentMappingElementToVkComponentSwizzle(Desc.ComponentMapping->R);
    Mapping.g = RHIComponentMappingElementToVkComponentSwizzle(Desc.ComponentMapping->G);
    Mapping.b = RHIComponentMappingElementToVkComponentSwizzle(Desc.ComponentMapping->B);
    Mapping.a = RHIComponentMappingElementToVkComponentSwizzle(Desc.ComponentMapping->A);
  } else {
    Mapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    Mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    Mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    Mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  }
  ImageViewInfo.components = Mapping;
  if (const VkResult Result = vkCreateImageView(GetVulkanGfxContexRef().GetDevice(), &ImageViewInfo, nullptr, &mImageView); Result != VK_SUCCESS) {
    gLogger.Error("RHI", "创建图像视图失败, 错误码={}", Result);
  }
}

RHIImageView_Vulkan::~RHIImageView_Vulkan() {
  if (mImageView) {
    vkDestroyImageView(GetVulkanGfxContexRef().GetDevice(), mImageView, nullptr);
    mImageView = nullptr;
  }
}