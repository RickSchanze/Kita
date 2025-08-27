//
// Created by kita on 25-8-27.
//

#include "CommandBuffer_Vulkan.h"

#include "GfxContext_Vulkan.h"

RHICommandPool_Vulkan::RHICommandPool_Vulkan(ERHIQueueFamilyType Family) {
  const auto& Ctx = GetVulkanGfxContexRef();
  const uint32_t FamilyIndex = Ctx.GetQueueFamilyIndex(Family);
  VkCommandPoolCreateInfo PoolInfo{};
  PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  PoolInfo.queueFamilyIndex = FamilyIndex;
  if (const VkResult Result = vkCreateCommandPool(Ctx.GetDevice(), &PoolInfo, nullptr, &mPool); Result != VK_SUCCESS) {
    LOG_ERROR_TAG("RHI.Vulkan", "创建CommandPool失败, 错误码={}", Result);
  }
  mFamily = Family;
}

RHICommandPool_Vulkan::~RHICommandPool_Vulkan() {
  vkDestroyCommandPool(GetVulkanGfxContexRef().GetDevice(), mPool, nullptr);
  mPool = nullptr;
}

UniquePtr<RHICommandBuffer> RHICommandPool_Vulkan::CreateCommandBuffer() {
  UniquePtr<RHICommandBuffer_Vulkan> CmdBuffer = MakeUnique<RHICommandBuffer_Vulkan>();
  VkCommandBufferAllocateInfo AllocInfo{};
  AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  AllocInfo.commandBufferCount = 1;
  AllocInfo.commandPool = mPool;
  AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  if (const VkResult Result = vkAllocateCommandBuffers(GetVulkanGfxContexRef().GetDevice(), &AllocInfo, &CmdBuffer->mBuffer); Result != VK_SUCCESS) {
    LOG_ERROR_TAG("RHI.Vulkan", "创建CommandBuffer失败, 错误码={}", Result);
  }
  CmdBuffer->mParentPool = this;
  return CmdBuffer;
}

void RHICommandPool_Vulkan::Reset() { vkResetCommandPool(GetVulkanGfxContexRef().GetDevice(), mPool, 0); }


TaskHandle RHICommandBuffer_Vulkan::Execute(StringView DebugName) {
  return {};
}
