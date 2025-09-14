//
// Created by kita on 25-9-13.
//

#include "ContentBrowserWindow.h"

#include "RHI/IconFontHeader.h"
#include "imgui_internal.h"

ContentBrowserWindow::ContentBrowserWindow() {
  mWindowTitle = "内容浏览器";
  mName = "ContentBrowserWindow";
  mShouldDeleteWhenUnVisible = false;
}

ContentBrowserWindow::~ContentBrowserWindow() {}

void ContentBrowserWindow::DrawEditorUI() {
  if (mRightPanelWidth == 0) {
    mRightPanelWidth = mWindowSize.X() - mLeftPanelWidth;
  }
  ImGui::BeginChild("A", {mLeftPanelWidth, 0});
  ImGui::Text("Hello World");
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::SameLine();

  EditorUI::Splitter(mLeftPanelWidth, mRightPanelWidth);

  ImGui::SameLine();
  ImGui::BeginChild("B", {mRightPanelWidth, 0});
  ImGui::Text("Hello World");
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::SameLine();
}