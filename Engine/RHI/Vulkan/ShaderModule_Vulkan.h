#pragma once
#include "RHI/ShaderModule.h"

#include <vulkan/vulkan_core.h>

class RHIShaderModule_Vulkan : public RHIShaderModule {
public:
  explicit RHIShaderModule_Vulkan(const RHIShaderModuleDesc& Desc);
  virtual ~RHIShaderModule_Vulkan() override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mModule; }

private:
  VkShaderModule mModule = VK_NULL_HANDLE;
};