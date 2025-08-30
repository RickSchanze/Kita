#pragma once
#include "Core/Memory/UniquePtr.h"
#include "Core/Singleton/Singleton.h"
#include "RenderPipeline.h"

class RenderTicker;

class RenderContext : public Singleton<RenderContext> {
public:
  RenderContext();

  void Render(double Time);

  bool ShouldRender() const;

private:
  UniquePtr<RenderTicker> mRenderTicker = {};

  /// 默认的RenderPipeline是一个只绘制ImGui的RenderPipeline
  UniquePtr<RenderPipeline> mRenderPipeline = {};
};
