//
// Created by kita on 25-8-30.
//

#include "RenderContext.h"

#include "Core/Performance/ProfilerMark.h"

RenderContext::RenderContext() { mRenderTicker = MakeUnique<RenderTicker>(); }

void RenderContext::Render(double Time) {
  if (ShouldRender())
    return;
  CPU_PROFILING_SCOPE;
  mRenderPipeline->Draw(Time);
}

bool RenderContext::ShouldRender() const { return mRenderPipeline; }