//
// Created by kita on 25-9-21.
//

#include "ViewportWindow.h"

#include "RHI/GfxContext.h"
#include "Render/RenderContext.h"

ViewportWindow::ViewportWindow() {
  mName = "ViewportWindow";
  mWindowTitle = "视口";
  mRenderPipelineRenderHandle = RenderContext::GetRef().OnRenderPipelineRender.Add(this, &ThisClass::Render);
}

ViewportWindow::~ViewportWindow() { RenderContext::GetRef().OnRenderPipelineRender.Remove(mRenderPipelineRenderHandle); }

void ViewportWindow::DrawEditorUI() { EditorUI::Text("视口"); }

void ViewportWindow::SetRenderPipeline(UniquePtr<RenderPipeline>&& NewRenderPipeline) {
  if (!NewRenderPipeline) {
    gLogger.Error(Logcat::Editor, "企图设置空RenderPipeline, 跳过此次操作.");
  }
  GetGfxContextRef().WaitDeviceIdle();
  mRenderPipeline = std::move(NewRenderPipeline);
}

void ViewportWindow::Render(const RenderPipelineDrawParams& Params) {
  if (mRenderPipeline) {
    mRenderPipeline->Draw(Params);
  }
}