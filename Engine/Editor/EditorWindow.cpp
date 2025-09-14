//
// Created by kita on 25-8-31.
//

#include "EditorWindow.h"

#include "EditorUI.h"

void EditorWindow::Render() {
  if (mShowWindow) {
    if (!mBeginEndWindowSelf) {
      EditorUI::Begin(mWindowTitle, &mShowWindow, mFlags);
      mWindowSize = EditorUI::GetContentRegionAvail();
    }
    DrawEditorUI();
    if (!mBeginEndWindowSelf) {
      EditorUI::End();
    }
  }
}

float EditorWindow::GetAvailableContentWidth() { return mWindowSize.X(); }

Vector2f EditorWindow::GetWindowSize() const { return mWindowSize; }