#pragma once
#include "IRHIResource.h"
#include "RHIEnums.h"
struct RHIDescriptorSetLayoutBinding {
  RHI_DEFINE_BUILDER_FIELD(UInt32, Binding, 0)
  RHI_DEFINE_BUILDER_FIELD(ERHIDescriptorType, DescriptorType, ERHIDescriptorType::Count)
  RHI_DEFINE_BUILDER_FIELD(UInt32, DescriptorCount, 1) // for shader array like Texture2D[] Texs;
  RHI_DEFINE_BUILDER_FIELD(ERHIShaderStage, Stage, SSB_MAX)
};

struct RHIDescriptorSetLayoutDesc {
  RHI_DEFINE_BUILDER_FIELD(std::vector<RHIDescriptorSetLayoutBinding>, Bindings, {})
};

class RHIDescriptorSetLayout : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::DescriptorSetLayout; }
};

class RHIDescriptorSet : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::DescriptorSet; }
};

struct RHIDescriptorPoolSize {
  ERHIDescriptorType Type;
  UInt32 Count; // 这是无绑定流程中数组的最大个数
};

struct RHIDescriptorPoolDesc {
  RHI_DEFINE_BUILDER_FIELD(UInt32, MaxSets, 10) // 最多由此Pool生成多少个DescriptorSet
  RHI_DEFINE_BUILDER_FIELD(Array<RHIDescriptorPoolSize>, PoolSizes, {})
};

struct RHIDescriptorSetsAllocInfo {
  RHI_DEFINE_BUILDER_FIELD(Array<RHIDescriptorSetLayout*>, DescriptorSetLayouts, {})
};

class RHIDescriptorPool : public IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override { return ERHIResourceType::DescriptorPool; }

  virtual Array<UniquePtr<RHIDescriptorSet>> CreateDescriptorSets(const RHIDescriptorSetsAllocInfo& AllocInfos) = 0;
};
