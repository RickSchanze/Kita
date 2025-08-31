#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/MenuAction.h"

#include "Core/Container/Deque.h"
#include "LoggingWindow.generated.h"

KCLASS()
class LoggingWindow : public EditorWindow {
  GENERATED_BODY(LoggingWindow)
public:
  LoggingWindow();
  ~LoggingWindow();
  virtual void Draw() override;

  static void RecordLog(const Log& Log);

  Int32 MaxLoggingRecordCount = 1000;
  Deque<Log> LoggingRecords;
  Logger::CallbackHandle LoggingCallbackHandle;
};

KCLASS(MenuActionItem = "工具/日志")
class MenuAction_OpenLoggingWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenLoggingWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<LoggingWindow>(); }
};
