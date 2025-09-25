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
  virtual void Draw(const RenderPipelineDrawParams& Params) = 0;

  void SetBackBuffer(const SharedPtr<RenderTarget>& BackBuffer) {
    mBackBuffer = BackBuffer;
    OnBackBufferSet();
  }

  SharedPtr<RenderTarget> GetBackBuffer() const { return mBackBuffer; }

  virtual void OnBackBufferSet() {}

  /// Resize你需要的东西 BackBuffer不需要你来Resize
  virtual void Resize(UInt32 NewWidth, UInt32 NewHeight) = 0;

private:
  SharedPtr<RenderTarget> mBackBuffer;
};
