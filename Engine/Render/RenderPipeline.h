#pragma once
#include "Core/Reflection/MetaMark.h"

#include "Core/TaskGraph/TaskHandle.h"
#include "RenderPipeline.generated.h"

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

KCLASS()
class RenderPipeline {
  GENERATED_BODY(RenderPipeline)
public:
  virtual ~RenderPipeline() = default;
  virtual void Draw(const RenderPipelineDrawParams& Params);
  void DrawImGui(const RenderPipelineDrawParams& Params);

#if KITA_EDITOR
protected:
  TaskHandle RecordImGuiCommands(const RenderPipelineDrawParams& Params);
#endif
};
