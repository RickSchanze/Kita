//
// Created by kita on 25-9-23.
//

#include "DefaultRenderPipeline.h"

#include "RHI/GfxContext.h"
#include "RHI/Image.h"

DefaultRenderPipeline::DefaultRenderPipeline() = default;

DefaultRenderPipeline::~DefaultRenderPipeline() {
  GfxContext::GetRef().WaitDeviceIdle();
  mDepthTarget = nullptr;
}

void DefaultRenderPipeline::Resize(UInt32 NewWidth, UInt32 NewHeight) {
  if (Width == NewWidth && Height == NewHeight) {
    return;
  }
  RHIImageDesc DepthTargetDesc = {};
  DepthTargetDesc.Width = NewWidth;
  DepthTargetDesc.Height = NewHeight;
  DepthTargetDesc.Format = ERHIFormat::D32_Float;
  DepthTargetDesc.ArrayLayers = 1;
  DepthTargetDesc.Dimension = ERHIImageDimension::D2;
  DepthTargetDesc.Depth = 1;
  DepthTargetDesc.Usage = ERHIImageUsage::DepthStencil;
  UniquePtr<RenderTarget> DepthTarget = MakeUnique<RenderTarget>(DepthTargetDesc);
  GetGfxContextRef().WaitDeviceIdle();
  mDepthTarget = std::move(DepthTarget);
}