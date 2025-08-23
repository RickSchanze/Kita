#pragma once
#include "Core/TypeDefs.h"
#include "IRHIResource.h"
#include "RHIFormat.h"

class Image;

struct ComponentMapping {
  RHI_DEFINE_BUILDER_FIELD(ERHIComponentMappingElement, R, ERHIComponentMappingElement::Identity);
  RHI_DEFINE_BUILDER_FIELD(ERHIComponentMappingElement, G, ERHIComponentMappingElement::Identity);
  RHI_DEFINE_BUILDER_FIELD(ERHIComponentMappingElement, B, ERHIComponentMappingElement::Identity);
  RHI_DEFINE_BUILDER_FIELD(ERHIComponentMappingElement, A, ERHIComponentMappingElement::Identity);
};

struct ImageSubresourceRange {
  RHI_DEFINE_BUILDER_FIELD(ERHIImageAspect, AspectMask, {}) // 必填
  RHI_DEFINE_BUILDER_FIELD(UInt32, BaseMipLevel, 0);
  RHI_DEFINE_BUILDER_FIELD(UInt32, LevelCount, 1);
  RHI_DEFINE_BUILDER_FIELD(UInt32, BaseArrayLayer, 0);
  RHI_DEFINE_BUILDER_FIELD(UInt32, LayerCount, 1);
};

struct ImageViewDesc {
  RHI_DEFINE_BUILDER_FIELD(SharedPtr<Image>, SourceImage, {});                    // 必填
  RHI_DEFINE_BUILDER_FIELD(ERHIImageDimension, ViewType, ERHIImageDimension::D2); // ImageView的类型, 默认为2D图像
  RHI_DEFINE_BUILDER_FIELD(ERHIFormat, Format, ERHIFormat::Count)                 // ImageView的格式, 填Count则会自动获取Image的格式
  RHI_DEFINE_BUILDER_FIELD(::ComponentMapping, ComponentMapping, {});             // ImageView的组件映射
  RHI_DEFINE_BUILDER_FIELD(ImageSubresourceRange, SubresourceRange, {});          // 必填
};

class ImageView : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::ImageView; }
  // 应返回关联的Image 如果是交换链图像应返回nullptr
  [[nodiscard]] virtual SharedPtr<Image> GetRelatedImage() const = 0;
};