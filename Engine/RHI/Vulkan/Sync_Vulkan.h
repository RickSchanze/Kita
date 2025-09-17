#pragma once
#include "Core/String/StringView.h"
#include "RHI/Sync.h"

#include <vulkan/vulkan_core.h>

class RHIFence_Vulkan final : public RHIFence {
public:
  explicit RHIFence_Vulkan(StringView DebugName);
  virtual ~RHIFence_Vulkan() override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mFence; }

  virtual void Wait(uint64_t Timeout) override;

  virtual void Reset() override;

private:
  VkFence mFence = VK_NULL_HANDLE;
};

class RHISemaphore_Vulkan final : public RHISemaphore {
public:
  RHISemaphore_Vulkan();
  virtual ~RHISemaphore_Vulkan() override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mSemaphore; }

private:
  VkSemaphore mSemaphore = VK_NULL_HANDLE;
};
