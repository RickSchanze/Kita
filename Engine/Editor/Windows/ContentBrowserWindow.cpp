//
// Created by kita on 25-9-13.
//

#include "ContentBrowserWindow.h"

#include "Editor/Selection.h"
#include "RHI/IconFontHeader.h"
#include "imgui_internal.h"

ContentBrowserWindow::ContentBrowserWindow() {
  mWindowTitle = "内容浏览器";
  mName = "ContentBrowserWindow";
  mShouldDeleteWhenUnVisible = false;
  Root = New<DirectoryTreeNode>();
  auto R1 = New<DirectoryTreeNode>();
  auto R2 = New<DirectoryTreeNode>();
  auto R3 = New<DirectoryTreeNode>();
  auto R4 = New<DirectoryTreeNode>();
  Root->Children.Add(R1);
  Root->Children.Add(R2);
  R2->Children.Add(R3);
  R3->Children.Add(R4);
  R4->Indent = 2;
  R1->Indent = 0;
  R2->Indent = 0;
  R3->Indent = 1;
  R1->Name = "Folder1";
  R2->Name = "Folder2";
  R3->Name = "Folder3";
  R4->Name = "Folder4";
  Root->IsOpened = true;
}

ContentBrowserWindow::~ContentBrowserWindow() {}

static void DrawDirectoryTreeRecursive(ContentBrowserWindow::DirectoryTreeNode* Root) {
  if (Root != nullptr) {
    if (Root->Name.Empty()) {
      for (const auto Node : Root->Children) {
        DrawDirectoryTreeRecursive(Node);
      }
    } else {
      for (Int32 Index = 0; Index < Root->Indent; Index++) {
        EditorUI::Indent();
      }
      ImGui::PushID(Root->Name.Data());
      const bool IsFolderButtonClicked = EditorUI::Button(Root->IsOpened ? KITA_ICON_FOLDER_OPEN : KITA_ICON_FOLDER);
      EditorUI::SameLine();
      const bool IsFolderNameClicked = EditorUI::Button(Root->Name);
      if (Root->IsOpened) {
        for (const auto Child : Root->Children) {
          DrawDirectoryTreeRecursive(Child);
        }
      }
      if (IsFolderButtonClicked) {
        if (!Root->Children.Empty()) {
          Root->IsOpened = !Root->IsOpened;
        }
      }
      if (IsFolderNameClicked) {
        // TODO: 修改gSelection
      }
      ImGui::PopID();
    }
  }
}

void ContentBrowserWindow::DrawEditorUI() {
  ImGui::BeginChild("A", {mLeftPanelWidth, 0});
  DrawDirectoryTreeRecursive(Root);
  ImGui::EndChild();
  ImGui::SameLine();
  EditorUI::Splitter(mLeftPanelWidth, mRightPanelWidth);
  ImGui::SameLine();

  ImGui::BeginChild("B", {mRightPanelWidth, 0});
  ImGui::Text("Hello World");
  ImGui::EndChild();
}