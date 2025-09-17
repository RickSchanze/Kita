//
// Created by kita on 25-8-24.
//

#include "Image_Vulkan.h"

#include "GfxContext_Vulkan.h"
#include "RHIEnums_Vulkan.h"

RHIImage_Vulkan::RHIImage_Vulkan(const RHIImageDesc& Desc) {
  // TODO: 实现
  mDesc = Desc;

  VkImageCreateInfo ImageInfo{};
  ImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  ImageInfo.imageType = RHIImageDimensionToVkImageType(Desc.Dimension);
  ImageInfo.extent.width = Desc.Width;
  ImageInfo.extent.height = Desc.Height;
  ImageInfo.extent.depth = Desc.Depth;
  ImageInfo.mipLevels = Desc.MipLevels;
  ImageInfo.arrayLayers = Desc.ArrayLayers;
  ImageInfo.format = RHIFormatToVkFormat(Desc.Format);
  ImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  ImageInfo.initialLayout = RHIImageLayoutToVkImageLayout(Desc.InitialLayout);
  ImageInfo.usage = RHIImageUsageToVkImageUsageFlags(Desc.Usage);
  ImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  ImageInfo.flags = 0;

  auto Device = GetVulkanGfxContexRef().GetDevice();
  if (auto Result = vkCreateImage(Device, &ImageInfo, nullptr, &mNativeImage); Result != VK_SUCCESS) {
    gLogger.Error(Logcat::RHI, "Failed to create image! '{}' code={}.", Desc.DebgName, Result);
    return;
  }

  mMemory = GetVulkanGfxContexRef().AllocateMemory(mNativeImage, RHIMemoryPropertyToVkMemoryProperty(ERHIBufferMemoryProperty::DeviceLocal));
  if (mMemory == nullptr) {
    gLogger.Error(Logcat::RHI, "Failed to allocate memory for image! '{}'.", Desc.DebgName);
    if (mNativeImage != VK_NULL_HANDLE) {
      vkDestroyImage(GetVulkanGfxContexRef().GetDevice(), mNativeImage, nullptr);
    }
    return;
  }
  vkBindImageMemory(GetVulkanGfxContexRef().GetDevice(), mNativeImage, mMemory, 0);
}

RHIImage_Vulkan::~RHIImage_Vulkan() {
  // TODO: 实现
  GetVulkanGfxContexRef().DeallocateMemory(mMemory);
  vkDestroyImage(GetVulkanGfxContexRef().GetDevice(), mNativeImage, nullptr);
  mNativeImage = VK_NULL_HANDLE;
  mMemory = nullptr;
}