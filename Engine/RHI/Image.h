#pragma once
#include "IRHIResource.h"
#include "RHIFormat.h"

struct ImageDesc {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Width, 0)  // necessary
  RHI_DEFINE_BUILDER_FIELD(UInt32, Height, 0) // necessary
  RHI_DEFINE_BUILDER_FIELD(UInt32, Depth, 1)
  RHI_DEFINE_BUILDER_FIELD(ERHIImageDimension, Dimension, ERHIImageDimension::D2) // 这个值可选: D1 D2 D3其他不可选
  RHI_DEFINE_BUILDER_FIELD(ERHIFormat, Format, ERHIFormat::Count)                 // necessary
  RHI_DEFINE_BUILDER_FIELD(UInt32, MipLevels, 1)
  RHI_DEFINE_BUILDER_FIELD(UInt32, ArrayLayers, 1)
  RHI_DEFINE_BUILDER_FIELD(ERHIImageUsage, Usage, ERHIImageUsageBits::IUB_Max) // necessary
  RHI_DEFINE_BUILDER_FIELD(ERHIImageLayout, InitialLayout, ERHIImageLayout::Undefined)

#ifdef KITA_DEBUG
  RHI_DEFINE_BUILDER_FIELD(std::string, DebgName, "<Unnamed Image>");
#endif
};

class Image : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::Image; }
  [[nodiscard]] virtual void* GetNativeHandle() const override = 0;
  [[nodiscard]] const ImageDesc& GetDesc() const { return mDesc; }

protected:
  ImageDesc mDesc{};
};
