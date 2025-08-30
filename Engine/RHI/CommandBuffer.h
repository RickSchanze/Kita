#pragma once
#include "Core/Container/Queue.h"
#include "Core/TaskGraph/TaskHandle.h"
#include "IRHIResource.h"
#include "Math/Color.h"
#include "Math/Vector.h"
#include "RHIEnums.h"

class RHIFrameBuffer;
class RHIRenderPass;
struct IRHICommand;
class RHISemaphore;
class RHIFence;
class RHICommandBuffer;

struct RHICommandBufferSubmitParams {
  RHI_DEFINE_BUILDER_FIELD(Array<RHISemaphore*>, WaitSemaphores, {})
  RHI_DEFINE_BUILDER_FIELD(ERHIPipelineStageFlags, WaitPipelineStages, {})
  RHI_DEFINE_BUILDER_FIELD(Array<RHISemaphore*>, SignalSemaphores, {})
  RHI_DEFINE_BUILDER_FIELD_PTR(RHIFence*, Fence, nullptr)
  RHI_DEFINE_BUILDER_FIELD(ERHIQueueFamilyType, TargetQueueFamily, ERHIQueueFamilyType::Graphics)
  RHI_DEFINE_BUILDER_FIELD_PTR(RHICommandBuffer*, CommandBuffer, nullptr)
};

class RHICommandBuffer : public IRHIResource {
public:
  virtual ~RHICommandBuffer() override;

  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::CommandBuffer; }
  virtual void Reset() = 0;

  void PushCommand(UniquePtr<IRHICommand>&& Cmd);

  void Clear();

  /**
   * 此函数的行为是将Queue的东西交给渲染线程进行渲染
   * @param DebugName
   */
  virtual TaskHandle Execute(StringView DebugName) = 0;

  void BeginRecord();
  void EndRecord();


  void BeginRenderPass(RHIRenderPass* RenderPass, RHIFrameBuffer* FrameBuffer, Vector2u Size, const Optional<Color>& ClearColor = {}, Vector2i Offset = {}, Optional<float> ClearDepth = std::nullopt);

  void EndRenderPass();

protected:
  // TODO: ByPass提交
  Queue<UniquePtr<IRHICommand>> mCommandQueue;
};

class RHICommandPool : IRHIResource {
public:
  [[nodiscard]] virtual ERHIResourceType GetResourceType() const override final { return ERHIResourceType::CommandPool; }

  virtual UniquePtr<RHICommandBuffer> CreateCommandBuffer() = 0;

  /**
   * 重置CommandPool 同时重置其创建的CommandBuffer
   * 此时CommandBuffer恢复到可录制的状态
   */
  virtual void Reset() = 0;

private:
};
