//
// Created by kita on 25-8-31.
//

#include "LoggingWindow.h"

#include "Core/Performance/ProfilerMark.h"
#include "RHI/IconFontHeader.h"

LoggingWindow::LoggingWindow() {
  mWindowTitle = "日志";
  mName = "LoggingWindow";
  mShouldDeleteWhenUnVisible = false;
  mLoggingCallbackHandle = gLogger.Evt_Log.Add(RecordLog);
}

LoggingWindow::~LoggingWindow() {
  gLogger.Evt_Log.Remove(mLoggingCallbackHandle);
  mLoggingCallbackHandle = {};
}

static void DrawLog(const Log& InLog) {
  for (int i = 0; i < 3; i++) {
    EditorUI::TableSetColumnIndex(i);
    switch (i) {
    case 0:
      EditorUI::Text(InLog.Tag.Data());
      break;
    case 1:
      switch (InLog.Level) {
      case ELogLevel::Critical:
        EditorUI::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, "Critical");
        break;
      case ELogLevel::Error:
        EditorUI::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, "Error");
        break;
      case ELogLevel::Info:
        EditorUI::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Info");
        break;
      case ELogLevel::Warn:
        EditorUI::TextColored({1.0f, 1.0f, 0.0f, 1.0f}, "Warn");
        break;
      }
      break;
    case 2:
      EditorUI::Text(InLog.Message.Data());
      break;
    default:
      std::unreachable();
    }
  }
}

static void DrawTagFilterCombo(Map<StringView, bool>& TagsFilter) {
  if (EditorUI::BeginCombo("标签筛选", "", EditorUI::EComboFlags::NoPreview)) {
    for (auto& [Tag, Enabled] : TagsFilter) {
      if (EditorUI::Selectable(Tag, Enabled, EditorUI::ESelectableFlags::NoAutoClosePopups)) {
        Enabled = !Enabled;
      }
      if (Enabled) {
        EditorUI::SameLine();
        EditorUI::Text(KITA_ICON_FONT_CHECK);
      }
    }
    EditorUI::EndCombo();
  }
}

void LoggingWindow::DrawEditorUI() {
  CPU_PROFILING_SCOPE;
  DrawTagFilterCombo(mLoggingRecordTagFilter);
  float ContentWidth = GetAvailableContentWidth();
  // 标签筛选
  constexpr EditorUI::ETableFlags Flags =
      EditorUI::ETableFlags::SizingStretchProp | EditorUI::ETableFlags::Borders | EditorUI::ETableFlags::Resizable | EditorUI::ETableFlags::RowBg | EditorUI::ETableFlags::BordersInner;
  if (EditorUI::BeginTable("LoggingTable", 3, Flags, Vector2f(ContentWidth, 0.0f))) {
    EditorUI::TableSetupColumn("Tag");
    EditorUI::TableSetupColumn("等级");
    EditorUI::TableSetupColumn("消息");
    EditorUI::TableHeadersRow();
    for (auto& Log : mLoggingRecords) {
      if (ShouldLogDraw(Log)) {
        EditorUI::TableNextRow();
        DrawLog(Log);
      }
    }
    EditorUI::EndTable();
  }
}

void LoggingWindow::RecordLog(const Log& Log) {
  static LoggingWindow* Window = static_cast<LoggingWindow*>(EditorWindowManager::GetWindow(LoggingWindow::GetStaticType()));
  if (Window == nullptr) {
    Window = static_cast<LoggingWindow*>(EditorWindowManager::GetWindow(LoggingWindow::GetStaticType()));
  }
  if (Window->mLoggingRecords.Count() > Window->mMaxLoggingRecordCount) {
    Window->mLoggingRecords.RemoveFront();
    Window->mLoggingRecords.Add(Log);
  } else {
    Window->mLoggingRecords.Add(Log);
  }
  if (!Window->mLoggingRecordTagFilter.Contains(Log.Tag)) {
    Window->mLoggingRecordTagFilter[Log.Tag] = true;
  }
}

bool LoggingWindow::ShouldLogDraw(const Log& Log) {
  if (mLoggingRecordTagFilter.Contains(Log.Tag)) {
    return mLoggingRecordTagFilter[Log.Tag];
  }
  return true;
}
