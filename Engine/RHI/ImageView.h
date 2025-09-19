#pragma once
#include "Core/TypeDefs.h"
#include "IRHIResource.h"
#include "RHIEnums.h"

class RHIImage;

struct RHIComponentMapping {
  RHI_DEFINE_BUILDER_FIELD(ERHIComponentMappingElement, R, ERHIComponentMappingElement::Identity);
  RHI_DEFINE_BUILDER_FIELD(ERHIComponentMappingElement, G, ERHIComponentMappingElement::Identity);
  RHI_DEFINE_BUILDER_FIELD(ERHIComponentMappingElement, B, ERHIComponentMappingElement::Identity);
  RHI_DEFINE_BUILDER_FIELD(ERHIComponentMappingElement, A, ERHIComponentMappingElement::Identity);
};

struct RHIImageSubresourceRange {
  RHI_DEFINE_BUILDER_FIELD(ERHIImageAspect, AspectMask, {}) // 必填
  RHI_DEFINE_BUILDER_FIELD(UInt32, BaseMipLevel, 0);
  RHI_DEFINE_BUILDER_FIELD(UInt32, LevelCount, 1);
  RHI_DEFINE_BUILDER_FIELD(UInt32, BaseArrayLayer, 0);
  RHI_DEFINE_BUILDER_FIELD(UInt32, LayerCount, 1);
};

struct RHIImageViewDesc {
  RHI_DEFINE_BUILDER_FIELD_PTR(RHIImage*, SourceImage, {});                           // 必填
  RHI_DEFINE_BUILDER_FIELD(ERHIImageDimension, ViewType, ERHIImageDimension::D2);     // ImageView的类型, 默认为2D图像
  RHI_DEFINE_BUILDER_FIELD(ERHIFormat, Format, ERHIFormat::Count)                     // ImageView的格式, 填Count则会自动获取Image的格式
  RHI_DEFINE_BUILDER_FIELD(Optional<RHIComponentMapping>, ComponentMapping, {});      // ImageView的组件映射
  RHI_DEFINE_BUILDER_FIELD(Optional<RHIImageSubresourceRange>, SubresourceRange, {}); // 必填
};

class RHIImageView : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::ImageView; }
};