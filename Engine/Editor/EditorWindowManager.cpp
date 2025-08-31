//
// Created by kita on 25-8-31.
//

#include "EditorWindowManager.h"

#include "EditorWindow.h"
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
    return NewWindow;
  }
  return nullptr;
}

void EditorWindowManager::Close(const Type* InType) {
  auto& Self = GetRef();
  if (Self.mEditorWindows.Contains(InType)) {
    Self.mEditorWindows[InType]->Hide();
    Delete(Self.mEditorWindows[InType]);
    Self.mEditorWindows.Remove(InType);
  }
}

void EditorWindowManager::StartUp() { Open<LoggingWindow>(); }

void EditorWindowManager::ShutDown() {
  for (auto& [Type, Window] : GetRef().mEditorWindows) {
    Delete(Window);
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