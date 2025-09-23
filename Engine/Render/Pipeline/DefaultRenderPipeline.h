#pragma once
#include "Render/RenderPipeline.h"

#include "DefaultRenderPipeline.generated.h"

KCLASS()
class DefaultRenderPipeline : public RenderPipeline {
public:
  DefaultRenderPipeline();
  virtual ~DefaultRenderPipeline() override;

  virtual void Resize(UInt32 NewWidth, UInt32 NewHeight) override;

private:
  UniquePtr<RenderTarget> mDepthTarget;

  UInt32 Width;
  UInt32 Height;
};
