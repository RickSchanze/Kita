//
// Created by kita on 25-9-23.
//

#include "DefaultRenderPipeline.h"

#include "RHI/GfxContext.h"
#include "RHI/Image.h"
#include "Render/RenderTarget.h"

DefaultRenderPipeline::DefaultRenderPipeline() = default;

DefaultRenderPipeline::~DefaultRenderPipeline() {
  GfxContext::GetRef().WaitDeviceIdle();
  mDepthTarget = nullptr;
}

void DefaultRenderPipeline::Resize(const UInt32 NewWidth, const UInt32 NewHeight) {
  if (mDepthTarget) {
    mDepthTarget->Resize(NewWidth, NewHeight);
  }
}

void DefaultRenderPipeline::Draw(const RenderPipelineDrawParams& Params) {}

void DefaultRenderPipeline::OnBackBufferSet() {
  RHIImageDesc ImageDesc{};
  RHIImageDesc DepthTargetDesc = {};
  if (GetBackBuffer()) {
    DepthTargetDesc.Width = GetBackBuffer()->GetWidth();
    DepthTargetDesc.Height = GetBackBuffer()->GetHeight();
  } else {
    DepthTargetDesc.Width = 1;
    DepthTargetDesc.Height = 1;
  }
  DepthTargetDesc.Format = ERHIFormat::D32_Float;
  DepthTargetDesc.ArrayLayers = 1;
  DepthTargetDesc.Dimension = ERHIImageDimension::D2;
  DepthTargetDesc.Depth = 1;
  DepthTargetDesc.Usage = ERHIImageUsage::DepthStencil;
  mDepthTarget = MakeUnique<RenderTarget>(DepthTargetDesc, ERHIImageLayout::DepthStencilAttachment, "DepthTarget");
}