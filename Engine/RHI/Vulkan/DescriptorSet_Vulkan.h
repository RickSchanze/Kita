#pragma once
#include "RHI/DescriptorSet.h"

#include <vulkan/vulkan_core.h>

class RHIDescriptorSetLayout_Vulkan : public RHIDescriptorSetLayout {
public:
  explicit RHIDescriptorSetLayout_Vulkan(const RHIDescriptorSetLayoutDesc& Desc);
  virtual ~RHIDescriptorSetLayout_Vulkan() override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mLayout; }

private:
  VkDescriptorSetLayout mLayout = VK_NULL_HANDLE;
};

class VulkanDescriptorSet : public RHIDescriptorSet {
public:
  explicit VulkanDescriptorSet(VkDescriptorSet Set) : mSet(Set) {}

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mSet; }

private:
  VkDescriptorSet mSet = VK_NULL_HANDLE;
};

class RHIDescriptorPool_Vulkan : public RHIDescriptorPool {
public:
  explicit RHIDescriptorPool_Vulkan(const RHIDescriptorPoolDesc& Desc);
  virtual ~RHIDescriptorPool_Vulkan() override;

  virtual Array<UniquePtr<RHIDescriptorSet>> CreateDescriptorSets(const RHIDescriptorSetsAllocInfo& AllocInfo) override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mPool; }

private:
  VkDescriptorPool mPool = VK_NULL_HANDLE;
};
