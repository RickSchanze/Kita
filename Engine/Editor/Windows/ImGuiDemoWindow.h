#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Editor/EditorWindow.h"
#include "Editor/MenuAction.h"

#include "ImGuiDemoWindow.generated.h"

KCLASS()
class ImGuiDemoWindow : public EditorWindow {
  GENERATED_BODY(ImGuiDemoWindow)
public:
  ImGuiDemoWindow();
  virtual void DrawEditorUI() override;
};

KCLASS(MenuActionItem = "示例/ImGui Demo")
class MenuAction_ShowImGuiDemo : public MenuAction {
  GENERATED_BODY(MenuAction_ShowImGuiDemo)
public:
  virtual void Execute() override { EditorWindowManager::Open<ImGuiDemoWindow>(); }
};
