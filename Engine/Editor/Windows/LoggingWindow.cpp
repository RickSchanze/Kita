//
// Created by kita on 25-8-31.
//

#include "LoggingWindow.h"

LoggingWindow::LoggingWindow() {
  mWindowTitle = "日志";
  mShouldDeleteWhenUnVisible = false;
  LoggingCallbackHandle = gLogger.Evt_Log.Add(RecordLog);
}

LoggingWindow::~LoggingWindow() {
  gLogger.Evt_Log.Remove(LoggingCallbackHandle);
  LoggingCallbackHandle = {};
}

void LoggingWindow::Draw() {
  EditorUI::Text("测试！");
}

void LoggingWindow::RecordLog(const Log& Log) {
  static LoggingWindow* Window = static_cast<LoggingWindow*>(EditorWindowManager::GetWindow(LoggingWindow::GetStaticType()));
  if (Window == nullptr) {
    Window = static_cast<LoggingWindow*>(EditorWindowManager::GetWindow(LoggingWindow::GetStaticType()));
  }
  if (Window->LoggingRecords.Count() > Window->MaxLoggingRecordCount) {
    Window->LoggingRecords.RemoveFront();
    Window->LoggingRecords.Add(Log);
  } else {
    Window->LoggingRecords.Add(Log);
  }
}
