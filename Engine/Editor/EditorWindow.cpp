//
// Created by kita on 25-8-31.
//

#include "EditorWindow.h"

#include "EditorUI.h"

#include <imgui.h>

void EditorWindow::Render() {
  if (mShowWindow) {
    EditorUI::Begin(mWindowTitle, &mShowWindow, mFlags);
    Draw();
    EditorUI::End();
  }
}