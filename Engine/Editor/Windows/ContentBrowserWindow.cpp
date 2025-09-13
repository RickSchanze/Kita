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
  EditorUI::BeginHorizontal("ContentBrowser");
  if (mLeftPanelWidth == 0) {
    mLeftPanelWidth = EditorUI::GetContentRegionAvail().X() / 2;
  }
  // 左侧面板
  EditorUI::BeginChild("Left", Vector2f(mLeftPanelWidth, 0));
  ImGui::Text("文件夹树");
  if (ImGui::Button(IsFolderOpen ? KITA_ICON_FOLDER_OPEN : KITA_ICON_FOLDER)) {
    IsFolderOpen = !IsFolderOpen;
  }
  ImGui::SameLine();
  if (ImGui::Button("Root")) {
    gLogger.Info(Logcat::Editor, "选中了");
  }
  if (IsFolderOpen) {
    ImGui::Indent();
    ImGui::Text("123");
  }
  EditorUI::EndChild();

  // 可拖拽分隔器
  EditorUI::DraggableSeparator("LeftSplitter", mLeftPanelWidth, 20, EditorUI::GetContentRegionAvail().X() - 10);

  // 右侧面板
  EditorUI::BeginChild("Right", {GetAvailableContentWidth() - mLeftPanelWidth, 0});
  ImGui::Text("属性面板");
  ImGui::Text("文件大小: 1024 KB");
  ImGui::Text("修改时间: 2024-01-01");
  EditorUI::EndChild();

  EditorUI::EndHorizontal();
}