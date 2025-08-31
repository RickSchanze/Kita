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
  virtual void Draw() override;
};

KCLASS(MenuActionItem = "工具/ImGui Demo")
class MenuAction_ShowImGuiDemo : public MenuAction {
  GENERATED_BODY(MenuAction_ShowImGuiDemo)
public:
  virtual void Execute() override { EditorWindowManager::Open<ImGuiDemoWindow>(); }
};

