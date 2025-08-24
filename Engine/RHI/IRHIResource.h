#pragma once

enum class ERHIResourceType {
  Image,
  ImageView,
  Sampler,
  Buffer,
  DescriptorSet,
  DescriptorSetLayout,
  PipelineLayout,
  Pipeline,
  FrameBuffer,
  RenderPass,
  ShaderModule,
  Swapchain,
  CommandPool,
  CommandBuffer,
  Fence,
  Semaphore,
  DescriptorPool,
};

class IRHIResource {
public:
  virtual ~IRHIResource() = default;

  [[nodiscard]] virtual ERHIResourceType GetResourceType() const = 0;
  [[nodiscard]] virtual void* GetNativeHandle() const = 0;

  template <typename T> T GetNativeHandleT() const { return static_cast<T>(GetNativeHandle()); }
};