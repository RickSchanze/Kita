//
// Created by kita on 25-8-31.
//

#include "EditorWindow.h"

#include "EditorUI.h"

#include <imgui.h>

void EditorWindow::Render() {
  if (mShowWindow) {
    if (!mBeginEndWindowSelf)
      EditorUI::Begin(mWindowTitle, &mShowWindow, mFlags);
    DrawEditorUI();
    if (!mBeginEndWindowSelf)
      EditorUI::End();
  }
}

float EditorWindow::GetAvailableContentWidth() { return EditorUI::GetContentRegionAvail().X(); }