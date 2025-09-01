//
// Created by kita on 25-8-31.
//

#include "ImGuiDemoWindow.h"

ImGuiDemoWindow::ImGuiDemoWindow() {
  mBeginEndWindowSelf = true;
  mWindowTitle = "ImGui Demo";
}

void ImGuiDemoWindow::DrawEditorUI() { ImGui::ShowDemoWindow(&mShowWindow); }