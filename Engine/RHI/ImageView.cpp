//
// Created by kita on 25-9-25.
//

#include "ImageView.h"

#include "Image.h"

RHIImageSubresourceRange GetImageSubresourceRangeFromImage(const RHIImage* Image) {
  if (Image == nullptr)
    return {};
  RHIImageSubresourceRange Range{};
  auto& Desc = Image->GetDesc();
  Range.SetBaseArrayLayer(0).SetLayerCount(Desc.ArrayLayers).SetBaseMipLevel(0).SetLevelCount(Desc.MipLevels);
  if (True(Desc.Usage & ERHIImageUsage::DepthStencil)) {
    if (Desc.Format == ERHIFormat::D32_Float) {
      Range.SetAspectMask(ERHIImageAspect::Depth);
    }
  }
  if (True(Desc.Usage & ERHIImageUsage::ShaderRead) || True(Desc.Usage & ERHIImageUsage::RenderTarget)) {
    Range.SetAspectMask(ERHIImageAspect::Color);
  }
  return Range;
}