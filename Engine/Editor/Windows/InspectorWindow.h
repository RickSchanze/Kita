#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/EditorWindowManager.h"
#include "Editor/MenuAction.h"

#include "InspectorWindow.generated.h"

KCLASS()
class InspectorWindow : public EditorWindow {
  GENERATED_BODY(InspectorWindow)
public:
  InspectorWindow();
  virtual ~InspectorWindow() override;
  virtual void DrawEditorUI() override;

private:
};

KCLASS(MenuActionItem = "窗口/细节")
class MenuAction_OpenInspectorWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenInspectorWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<InspectorWindow>(); }
};
