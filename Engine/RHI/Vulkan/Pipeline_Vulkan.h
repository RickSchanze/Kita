#pragma once
#include "RHI/Pipeline.h"

#include <vulkan/vulkan_core.h>

class RHIPipelineLayout_Vulkan : public RHIPipelineLayout {
public:
  explicit RHIPipelineLayout_Vulkan(const RHIPipelineLayoutDesc& Desc);
  virtual ~RHIPipelineLayout_Vulkan() override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mLayout; }

private:
  VkPipelineLayout mLayout;
};

class RHIPipeline_Vulkan : public RHIPipeline {
public:
  explicit RHIPipeline_Vulkan(const RHIGraphicsPipelineDesc& Desc);
  virtual ~RHIPipeline_Vulkan() override;

  [[nodiscard]] virtual void* GetNativeHandle() const override { return mPipeline; }

private:
  VkPipeline mPipeline = VK_NULL_HANDLE;
};
