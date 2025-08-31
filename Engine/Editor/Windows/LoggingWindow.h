#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/MenuAction.h"

#include "LoggingWindow.generated.h"

KCLASS()
class LoggingWindow : public EditorWindow {
  GENERATED_BODY(LoggingWindow)
};

KCLASS(MenuActionItem = "工具/日志")
class MenuAction_OpenLoggingWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenLoggingWindow)
public:
  virtual void Execute() override { LOG_INFO("Test", "打开日志！"); }
};
