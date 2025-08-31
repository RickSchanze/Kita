#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/MenuAction.h"

#include "LoggingWindow.generated.h"

KCLASS()
class LoggingWindow : public EditorWindow {
  GENERATED_BODY(LoggingWindow)
public:
  virtual void Draw() override;
};

KCLASS(MenuActionItem = "工具/日志")
class MenuAction_OpenLoggingWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenLoggingWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<LoggingWindow>(); }
};
