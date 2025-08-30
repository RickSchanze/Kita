#pragma once
#include "Core/Reflection/MetaMark.h"

#include "RenderPipeline.generated.h"

KCLASS()
class RenderPipeline {
public:
  virtual ~RenderPipeline() = default;
  virtual void Draw(double DeltaSeconds);

#if KITA_EDITOR
protected:
  void DrawImGui();

  UniquePtr<class RHIRenderPass> mImGuiRenderPass;
#endif
};
