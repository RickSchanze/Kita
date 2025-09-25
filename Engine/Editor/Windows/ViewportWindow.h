#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/MenuAction.h"
#include "Render/RenderContext.h"

#include "ViewportWindow.generated.h"

struct RenderPipelineDrawParams;
class RenderPipeline;
class RenderTarget;
KCLASS()
class ViewportWindow : public EditorWindow {
  GENERATED_BODY(ViewportWindow)
public:
  ViewportWindow();
  virtual ~ViewportWindow() override;
  virtual void DrawEditorUI() override;

  void SetRenderPipeline(UniquePtr<RenderPipeline>&& NewRenderPipeline);

  void Render(const RenderPipelineDrawParams& Params);

protected:
  void CreateBackBuffer(UInt32 W, UInt32 H);

  SharedPtr<RenderTarget> mBackBuffer;
  Vector2f mWindowSize;
  UniquePtr<RenderPipeline> mRenderPipeline;
  RenderContext::CHT_OnRenderPipelineRender mRenderPipelineRenderHandle;
  void* mImGuiBackBufferID = nullptr;
  bool mWindowResized;
};

KCLASS(MenuActionItem = "窗口/视口")
class MenuAction_OpenViewportWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenViewportWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<ViewportWindow>(); }
};
