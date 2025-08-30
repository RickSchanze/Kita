//
// Created by kita on 25-8-26.
//

#include "Sync_Vulkan.h"

#include "GfxContext_Vulkan.h"
RHIFence_Vulkan::RHIFence_Vulkan() {
  VkFenceCreateInfo CreateInfo{};
  CreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  CreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  if (VkResult Result = vkCreateFence(GetVulkanGfxContexRef().GetDevice(), &CreateInfo, nullptr, &mFence); Result != VK_SUCCESS) {
    LOG_ERROR_TAG("RHI.Vulkan", "创建Fence失败, Code={}", Result);
  }
}

RHIFence_Vulkan::~RHIFence_Vulkan() { vkDestroyFence(GetVulkanGfxContexRef().GetDevice(), mFence, nullptr); }

void RHIFence_Vulkan::Wait(const uint64_t Timeout) { vkWaitForFences(GetVulkanGfxContexRef().GetDevice(), 1, &mFence, VK_TRUE, Timeout); }

void RHIFence_Vulkan::Reset() { vkResetFences(GetVulkanGfxContexRef().GetDevice(), 1, &mFence); }

RHISemaphore_Vulkan::RHISemaphore_Vulkan() {
  VkSemaphoreCreateInfo CreateInfo{};
  CreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  if (VkResult Result = vkCreateSemaphore(GetVulkanGfxContexRef().GetDevice(), &CreateInfo, nullptr, &mSemaphore); Result != VK_SUCCESS) {
    LOG_ERROR_TAG("RHI.Vulkan", "创建Semaphore失败, Code={}", Result);
    mSemaphore = nullptr;
  }
}

RHISemaphore_Vulkan::~RHISemaphore_Vulkan() { vkDestroySemaphore(GetVulkanGfxContexRef().GetDevice(), mSemaphore, nullptr); }