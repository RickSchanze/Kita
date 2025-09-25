//
// Created by kita on 25-9-21.
//

#include "ViewportWindow.h"

#include "RHI/CommandBuffer.h"
#include "RHI/GfxContext.h"
#include "RHI/Image.h"
#include "Render/Pipeline/DefaultRenderPipeline.h"
#include "Render/RenderContext.h"
#include "Render/RenderTarget.h"

ViewportWindow::ViewportWindow() {
  mName = "ViewportWindow";
  mWindowTitle = "视口";
  mRenderPipelineRenderHandle = RenderContext::GetRef().OnRenderPipelineRender.Add(this, &ThisClass::Render);
  mRenderPipeline = MakeUnique<DefaultRenderPipeline>();
}

ViewportWindow::~ViewportWindow() {
  RenderContext::GetRef().OnRenderPipelineRender.Remove(mRenderPipelineRenderHandle);
  if (mImGuiBackBufferID != nullptr) {
    GetGfxContextRef().DestroyImGuiTexture(mImGuiBackBufferID);
  }
}

void ViewportWindow::DrawEditorUI() {
  auto NewSize = EditorUI::GetContentRegionAvail();
  if (NewSize != mWindowSize) {
    mWindowSize = NewSize;
    mWindowResized = true;
  }
  EditorUI::Image(mImGuiBackBufferID, mWindowSize);
}

void ViewportWindow::SetRenderPipeline(UniquePtr<RenderPipeline>&& NewRenderPipeline) {
  if (!NewRenderPipeline) {
    gLogger.Error(Logcat::Editor, "企图设置空RenderPipeline, 跳过此次操作.");
  }
  GetGfxContextRef().WaitDeviceIdle();
  mRenderPipeline = std::move(NewRenderPipeline);
  mRenderPipeline->SetBackBuffer(mBackBuffer);
  mRenderPipeline->Resize(static_cast<UInt32>(mWindowSize.X()), static_cast<UInt32>(mWindowSize.Y()));
}

void ViewportWindow::Render(const RenderPipelineDrawParams& Params) {
  if (mBackBuffer == nullptr) {
    CreateBackBuffer(static_cast<UInt32>(mWindowSize.X()), static_cast<UInt32>(mWindowSize.Y()));
    if (mBackBuffer == nullptr) {
      return;
    }
  }
  if (mRenderPipeline) {
    if (mWindowResized) {
      mRenderPipeline->Resize(static_cast<UInt32>(mWindowSize.X()), static_cast<UInt32>(mWindowSize.Y()));
      mWindowResized = false;
    }
    Params.Cmd->ResourceBarrier(mBackBuffer->GetImage(), ERHIImageLayout::ShaderReadOnly, ERHIImageLayout::ColorAttachment);
    mRenderPipeline->Draw(Params);
    Params.Cmd->ResourceBarrier(mBackBuffer->GetImage(), ERHIImageLayout::ColorAttachment, ERHIImageLayout::ShaderReadOnly);
  }
}

void ViewportWindow::CreateBackBuffer(UInt32 W, UInt32 H) {
  if (W == 0 || H == 0)
    return;
  RHIImageDesc BackBufferDesc{};
  BackBufferDesc.Format = ERHIFormat::R8G8B8A8_UNorm;
  BackBufferDesc.Width = W;
  BackBufferDesc.Height = H;
  BackBufferDesc.Usage = ERHIImageUsage::ShaderRead | ERHIImageUsage::RenderTarget;
  mBackBuffer = MakeShared<RenderTarget>(BackBufferDesc, ERHIImageLayout::ShaderReadOnly, "BackBuffer");
  if (mRenderPipeline) {
    mRenderPipeline->SetBackBuffer(mBackBuffer);
  }
  if (mImGuiBackBufferID != nullptr) {
    GetGfxContextRef().DestroyImGuiTexture(mImGuiBackBufferID);
  }
  mImGuiBackBufferID = GetGfxContextRef().CreateImGuiTexture(EditorUI::GetEditorUsedSampler(), mBackBuffer->GetView());
}