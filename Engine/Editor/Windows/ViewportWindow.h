#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/MenuAction.h"

#include "ViewportWindow.generated.h"

KCLASS()
class ViewportWindow : public EditorWindow {
  GENERATED_BODY(ViewportWindow)
public:
  ViewportWindow();
  virtual ~ViewportWindow() override;
  virtual void DrawEditorUI() override;

private:

};

KCLASS(MenuActionItem = "窗口/视口")
class MenuAction_OpenViewportWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenViewportWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<ViewportWindow>(); }
};

