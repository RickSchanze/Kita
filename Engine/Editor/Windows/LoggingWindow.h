#pragma once
#include "Core/Container/Deque.h"
#include "Editor/EditorWindow.h"
#include "Editor/MenuAction.h"

#include "LoggingWindow.generated.h"

KCLASS()
class LoggingWindow : public EditorWindow {
  GENERATED_BODY(LoggingWindow)
public:
  LoggingWindow();
  virtual ~LoggingWindow() override;
  virtual void DrawEditorUI() override;

  static void RecordLog(const Log& Log);
  bool ShouldLogDraw(const Log& Log);

private:
  Int32 mMaxLoggingRecordCount = 1000;
  Deque<Log> mLoggingRecords;
  Logger::CallbackHandle mLoggingCallbackHandle;
  Map<StringView, bool> mLoggingRecordTagFilter;
};

KCLASS(MenuActionItem = "窗口/日志")
class MenuAction_OpenLoggingWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenLoggingWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<LoggingWindow>(); }
};
