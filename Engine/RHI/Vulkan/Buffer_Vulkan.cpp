//
// Created by kita on 25-9-1.
//

#include "Buffer_Vulkan.h"

#include "GfxContext_Vulkan.h"
#include "RHIEnums_Vulkan.h"

RHIBuffer_Vulkan::RHIBuffer_Vulkan(const RHIBufferDesc& Desc) {
  mDesc = Desc;
  if (Desc.Size == 0) {
    return; // 无效Buffer
  }
  auto& Context = GetVulkanGfxContexRef();
  VkDevice Device = Context.GetDevice();
  VkBufferCreateInfo BufferInfo = {};
  BufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  BufferInfo.size = Desc.Size;
  BufferInfo.usage = RHIBufferUsageToVkBufferUsage(Desc.Usage);
  BufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vkCreateBuffer(Device, &BufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
    mBuffer = nullptr;
    mBufferMemory = nullptr;
    gLogger.Error("RHI", "创建Buffer失败!");
    return;
  }
  mBufferMemory = Context.AllocateMemory(mBuffer, Desc.Size, RHIBufferMemoryPropertyToVkMemoryPropertyFlags(Desc.MemoryProperty));
  if (mBufferMemory == nullptr) {
    vkDestroyBuffer(Device, mBuffer, nullptr);
    mBuffer = nullptr;
    gLogger.Error("RHI", "创建Buffer内存失败!");
    return;
  }
  vkBindBufferMemory(Device, mBuffer, mBufferMemory, 0);
}

RHIBuffer_Vulkan::~RHIBuffer_Vulkan() {
  const auto& Context = GetVulkanGfxContexRef();
  Context.DeallocateMemory(mBufferMemory);
  vkDestroyBuffer(Context.GetDevice(), mBuffer, nullptr);
}

void* RHIBuffer_Vulkan::Map(const UInt64 Size, const UInt64 Offset) {
  ASSERT_MSG(Offset + Size <= mDesc.Size, "Buffer Map内存越界!");
  void* MappedPtr = nullptr;
  vkMapMemory(GetVulkanGfxContexRef().GetDevice(), mBufferMemory, Offset, Size, 0, &MappedPtr);
  return MappedPtr;
}

void RHIBuffer_Vulkan::Unmap(void* MappedPtr) { vkUnmapMemory(GetVulkanGfxContexRef().GetDevice(), mBufferMemory); }