#pragma once
#include "IRHIResource.h"
#include "RHIEnums.h"

struct RHIImageDesc {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Width, 0)  // necessary
  RHI_DEFINE_BUILDER_FIELD(UInt32, Height, 0) // necessary
  RHI_DEFINE_BUILDER_FIELD(UInt32, Depth, 1)
  RHI_DEFINE_BUILDER_FIELD(ERHIImageDimension, Dimension, ERHIImageDimension::D2) // 这个值可选: D1 D2 D3其他不可选
  RHI_DEFINE_BUILDER_FIELD(ERHIFormat, Format, ERHIFormat::Count)                 // necessary
  RHI_DEFINE_BUILDER_FIELD(UInt32, MipLevels, 1)
  RHI_DEFINE_BUILDER_FIELD(UInt32, ArrayLayers, 1)
  RHI_DEFINE_BUILDER_FIELD(ERHIImageUsage, Usage, ERHIImageUsage::Max) // necessary
  RHI_DEFINE_BUILDER_FIELD(ERHIImageLayout, InitialLayout, ERHIImageLayout::Undefined)

#if KITA_DEBUG_NAME
  RHI_DEFINE_BUILDER_FIELD(String, DebgName, "<Unnamed Image>");
#endif
};

inline bool operator==(const RHIImageDesc& lhs, const RHIImageDesc& rhs) {
  return lhs.Width == rhs.Width && lhs.Height == rhs.Height && lhs.Depth == rhs.Depth && lhs.Dimension == rhs.Dimension && lhs.Format == rhs.Format && lhs.MipLevels == rhs.MipLevels &&
         lhs.ArrayLayers == rhs.ArrayLayers && lhs.Usage == rhs.Usage && lhs.InitialLayout == rhs.InitialLayout;
}

class RHIImage : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::Image; }
  [[nodiscard]] virtual void* GetNativeHandle() const override = 0;
  [[nodiscard]] const RHIImageDesc& GetDesc() const { return mDesc; }

protected:
  RHIImageDesc mDesc{};
};

struct RHISamplerDesc {
  // 放大 / 缩小过滤模式（必要）
  RHI_DEFINE_BUILDER_FIELD(ERHIFilterMode, MagFilter, ERHIFilterMode::Linear) // VK_FILTER_LINEAR
  RHI_DEFINE_BUILDER_FIELD(ERHIFilterMode, MinFilter, ERHIFilterMode::Linear) // VK_FILTER_LINEAR

  // Mipmap 模式
  RHI_DEFINE_BUILDER_FIELD(ERHIMipmapMode, MipmapMode, ERHIMipmapMode::Linear) // VK_SAMPLER_MIPMAP_MODE_LINEAR

  // U/V/W 轴寻址模式（必要）
  RHI_DEFINE_BUILDER_FIELD(ERHISamplerAddressMode, AddressModeU, ERHISamplerAddressMode::Repeat)
  RHI_DEFINE_BUILDER_FIELD(ERHISamplerAddressMode, AddressModeV, ERHISamplerAddressMode::Repeat)
  RHI_DEFINE_BUILDER_FIELD(ERHISamplerAddressMode, AddressModeW, ERHISamplerAddressMode::Repeat)

  // Mipmap LOD 偏移
  RHI_DEFINE_BUILDER_FIELD(float, MipLodBias, 0.0f)

  // 各向异性过滤
  RHI_DEFINE_BUILDER_FIELD(bool, AnisotropyEnable, false)
  RHI_DEFINE_BUILDER_FIELD(float, MaxAnisotropy, 1.0f)

  // 比较采样（用于阴影贴图）
  RHI_DEFINE_BUILDER_FIELD(bool, CompareEnable, false)
  RHI_DEFINE_BUILDER_FIELD(ERHICompareOp, CompareOp, ERHICompareOp::Always)

  // Mipmap LOD 范围
  RHI_DEFINE_BUILDER_FIELD(float, MinLod, 0.0f)
  RHI_DEFINE_BUILDER_FIELD(float, MaxLod, 0.0f)

  // 纹理边界颜色
  RHI_DEFINE_BUILDER_FIELD(ERHIBorderColor, BorderColor, ERHIBorderColor::FloatTransparentBlack)

  // 是否使用非归一化纹理坐标
  RHI_DEFINE_BUILDER_FIELD(bool, UnnormalizedCoordinates, false)

#ifdef KITA_DEBUG
  RHI_DEFINE_BUILDER_FIELD(String, DebugName, "<Unnamed Sampler>");
#endif
};

class RHISampler : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::Sampler; }
  [[nodiscard]] virtual void* GetNativeHandle() const override = 0;
};
