//
// Created by kita on 25-8-26.
//

#include "DescriptorSet_Vulkan.h"

#include "GfxContext_Vulkan.h"
#include "RHIEnums_Vulkan.h"

RHIDescriptorSetLayout_Vulkan::RHIDescriptorSetLayout_Vulkan(const RHIDescriptorSetLayoutDesc& Desc) {
  VkDescriptorSetLayoutCreateInfo CreateInfo = {};
  CreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  Array<VkDescriptorSetLayoutBinding> Bindings;
  Bindings.Reserve(Desc.GetBindings().Count());
  for (auto& Binding : Desc.GetBindings()) {
    VkDescriptorSetLayoutBinding NewBinding;
    NewBinding.binding = Binding.Binding;
    NewBinding.descriptorCount = 1;
    NewBinding.descriptorType = RHIDescriptorTypeToVkDescriptorType(Binding.DescriptorType);
    NewBinding.stageFlags = RHIShaderStageToVkShaderStage(Binding.Stage);
    NewBinding.pImmutableSamplers = nullptr;
    Bindings.Add(NewBinding);
  }
  CreateInfo.pBindings = Bindings.Data();
  if (const auto Result = vkCreateDescriptorSetLayout(GetVulkanGfxContexRef().GetDevice(), &CreateInfo, nullptr, &mLayout); Result != VK_SUCCESS) {
    mLayout = VK_NULL_HANDLE;
    gLogger.Error("RHI.Vulkan", "创建DescriptorSetLayout失败, Code={}", Result);
  }
}

RHIDescriptorSetLayout_Vulkan::~RHIDescriptorSetLayout_Vulkan() {
  if (mLayout) {
    vkDestroyDescriptorSetLayout(GetVulkanGfxContexRef().GetDevice(), mLayout, nullptr);
    mLayout = nullptr;
  }
}

RHIDescriptorPool_Vulkan::RHIDescriptorPool_Vulkan(const RHIDescriptorPoolDesc& Desc) {
  VkDescriptorPoolCreateInfo CreateInfo = {};
  CreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  CreateInfo.maxSets = Desc.MaxSets;
  Array<VkDescriptorPoolSize> PoolSizes;
  PoolSizes.Reserve(Desc.PoolSizes.Count());
  for (const auto& [Type, Count] : Desc.PoolSizes) {
    VkDescriptorPoolSize NewPoolSize;
    NewPoolSize.type = RHIDescriptorTypeToVkDescriptorType(Type);
    NewPoolSize.descriptorCount = Count;
    PoolSizes.Add(NewPoolSize);
  }
  CreateInfo.pPoolSizes = PoolSizes.Data();
  CreateInfo.poolSizeCount = PoolSizes.Count();
  if (const auto Result = vkCreateDescriptorPool(GetVulkanGfxContexRef().GetDevice(), &CreateInfo, nullptr, &mPool); Result != VK_SUCCESS) {
    mPool = VK_NULL_HANDLE;
    gLogger.Error("RHI.Vulkan", "创建DescriptorPool失败, 错误码={}", Result);
  }
}

RHIDescriptorPool_Vulkan::~RHIDescriptorPool_Vulkan() {
  if (mPool) {
    vkDestroyDescriptorPool(GetVulkanGfxContexRef().GetDevice(), mPool, nullptr);
    mPool = nullptr;
  }
}

Array<UniquePtr<RHIDescriptorSet>> RHIDescriptorPool_Vulkan::CreateDescriptorSets(const RHIDescriptorSetsAllocInfo& AllocInfo) {
  const auto Layouts = //
      AllocInfo.DescriptorSetLayouts | Ranges::Views::Transform([](const RHIDescriptorSetLayout* Layout) { return Layout->GetNativeHandleT<VkDescriptorSetLayout>(); }) |
      Ranges::To<Array<VkDescriptorSetLayout>>();
  Array<VkDescriptorSet> RawSets;
  VkDescriptorSetAllocateInfo AllocateInfo{};
  AllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  AllocateInfo.descriptorPool = mPool;
  AllocateInfo.descriptorSetCount = Layouts.Count();
  AllocateInfo.pSetLayouts = Layouts.Data();
  if (const auto Result = vkAllocateDescriptorSets(GetVulkanGfxContexRef().GetDevice(), &AllocateInfo, RawSets.Data()); Result != VK_SUCCESS) {
    gLogger.Error("RHI.Vulkan", "创建DescriptorSet失败, 错误码={}", Result);
    return {};
  }

  auto Sets = RawSets | Ranges::Views::Transform([](const VkDescriptorSet& Set) -> UniquePtr<RHIDescriptorSet> { return MakeUnique<VulkanDescriptorSet>(Set); }) |
              Ranges::To<Array<UniquePtr<RHIDescriptorSet>>>();
  return Sets;
}
