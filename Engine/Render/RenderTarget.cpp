//
// Created by kita on 25-9-21.
//

#include "RenderTarget.h"

#include "RHI/FrameBuffer.h"
#include "RHI/GfxContext.h"
#include "RHI/Image.h"
#include "RHI/ImageView.h"
#include "RenderContext.h"

RenderTarget::RenderTarget(const RHIImageDesc& ImageDesc) { Recreate(ImageDesc); }

RenderTarget::~RenderTarget() {
  mFrameBuffer = nullptr;
  mView = nullptr;
  mImage = nullptr;
}

void RenderTarget::Recreate(const RHIImageDesc& ImageDesc) {
  if (mImage) {
    if (mImage->GetDesc() == ImageDesc) {
      return;
    }
  }
  UniquePtr<RHIImage> NewImage = GfxContext::GetRef().CreateImageU(ImageDesc);
  RHIImageViewDesc ViewDesc{};
  ViewDesc.SourceImage = NewImage.Get();
  UniquePtr<RHIImageView> NewView = GfxContext::GetRef().CreateImageViewU(ViewDesc);
  mImage = std::move(NewImage);
  mView = std::move(NewView);
  mFrameBuffer = nullptr;
}

void RenderTarget::Resize(const UInt32 NewWidth, const UInt32 NewHeight) {
  auto Desc = mImage->GetDesc();
  if (Desc.Width == NewWidth && Desc.Height == NewHeight) {
    return;
  }
  Desc.Width = NewWidth;
  Desc.Height = NewHeight;
  Recreate(Desc);
}

bool RenderTarget::SetRenderPass(RHIRenderPass* RenderPass) {
  if (mFrameBuffer) {
    if (mFrameBuffer->GetBoundRenderPass() == RenderPass) {
      return true;
    }
  }
  if (mImage) {
    RHIFrameBufferDesc Desc{};
    const auto& ImageDesc = mImage->GetDesc();
    Desc.SetWidth(ImageDesc.Width);
    Desc.SetHeight(ImageDesc.Height);
    Desc.SetLayers(ImageDesc.ArrayLayers);
    Desc.SetRenderPass(RenderPass);
    Desc.Attachments.Add(mView.Get());
    auto NewFrameBuffer = GfxContext::GetRef().CreateFrameBufferU(Desc);
    GfxContext::GetRef().WaitDeviceIdle();
    mFrameBuffer = std::move(NewFrameBuffer);
    return true;
  } else {
    gLogger.Error(Logcat::Render, "RenderTarget::SetRenderPass: Image is null");
    return false;
  }
}

UInt32 RenderTarget::GetWidth() const { return mImage->GetDesc().Width; }
UInt32 RenderTarget::GetHeight() const { return mImage->GetDesc().Height; }