#pragma once
#include "Core/Reflection/MetaMark.h"

#include "Core/TaskGraph/TaskHandle.h"
#include "RenderPipeline.generated.h"

class RenderTarget;
class RHIFrameBuffer;
class RHISurfaceWindow;
class RHICommandBuffer;
struct RenderPipelineDrawParams {
  double DeltaSeconds;
  RHICommandBuffer* Cmd;
  RHIFrameBuffer* TargetFramebuffer;
  UInt32 Width;
  UInt32 Height;
};

KCLASS(Abstract)
class RenderPipeline {
  GENERATED_BODY(RenderPipeline)
public:
  virtual ~RenderPipeline() = default;
  virtual void Draw(const RenderPipelineDrawParams& Params);
  void DrawImGui(const RenderPipelineDrawParams& Params);

  void SetBackBuffer(const SharedPtr<RenderTarget>& BackBuffer) { mBackBuffer = BackBuffer; }
  SharedPtr<RenderTarget> GetBackBuffer() const { return mBackBuffer; }

  /// Resize你需要的东西 BackBuffer不需要你来Resize
  virtual void Resize(UInt32 NewWidth, UInt32 NewHeight) = 0;

#if KITA_EDITOR
protected:
  TaskHandle RecordImGuiCommands(const RenderPipelineDrawParams& Params);
#endif

private:
  SharedPtr<RenderTarget> mBackBuffer;
};
