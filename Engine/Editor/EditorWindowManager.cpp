//
// Created by kita on 25-8-31.
//

#include "EditorWindowManager.h"

#include "EditorWindow.h"
#include "Windows/ContentBrowserWindow.h"
#include "Windows/InspectorWindow.h"
#include "Windows/LoggingWindow.h"

EditorWindow* EditorWindowManager::Open(const Type* InType) {
  auto& Self = GetRef();
  if (Self.mEditorWindows.Contains(InType)) {
    Self.mEditorWindows[InType]->Show();
    return Self.mEditorWindows[InType];
  }
  if (EditorWindow* NewWindow = InType->CreateInstanceT<EditorWindow>()) {
    Self.mEditorWindows.Add(InType, NewWindow);
    NewWindow->Show();
    gLogger.Info("Editor", "创建窗口'{}'.", NewWindow->GetWindowTitle());
    return NewWindow;
  }
  return nullptr;
}

void EditorWindowManager::Close(const Type* InType) {
  auto& Self = GetRef();
  if (Self.mEditorWindows.Contains(InType)) {
    Self.mEditorWindows[InType]->Hide();
    gLogger.Info("Editor", "销毁窗口'{}'.", Self.mEditorWindows[InType]->GetWindowTitle());
    Delete(Self.mEditorWindows[InType]);
    Self.mEditorWindows.Remove(InType);
  }
}

void EditorWindowManager::StartUp() {
  Open<LoggingWindow>();
  Open<ContentBrowserWindow>();
  Open<InspectorWindow>();
}

void EditorWindowManager::ShutDown() {
  while (!GetRef().mEditorWindows.Empty()) {
    Close(GetRef().mEditorWindows.begin()->first);
  }
  GetRef().mEditorWindows.Clear();
}

void EditorWindowManager::Render() {
  Array<const Type*> RemovedWindows;
  for (auto& [Type, Window] : GetRef().mEditorWindows) {
    if (Window->IsVisible()) {
      Window->Render();
    } else {
      if (Window->ShouldDeleteWhenUnVisible())
        RemovedWindows.Add(Window->GetType());
    }
  }
  for (auto& Window : RemovedWindows) {
    Close(Window);
  }
}

EditorWindow* EditorWindowManager::GetWindow(const Type* WindowType) {
  if (GetRef().mEditorWindows.Contains(WindowType)) {
    return GetRef().mEditorWindows[WindowType];
  }
  return nullptr;
}