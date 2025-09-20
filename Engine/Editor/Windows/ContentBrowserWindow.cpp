//
// Created by kita on 25-9-13.
//

#include "ContentBrowserWindow.h"

#include "Assets/AssetsManager.h"
#include "Core/String/StringUtils.h"
#include "Editor/Selection.h"
#include "RHI/IconFontHeader.h"

static void BuildAssetTreeNodeFromIndex(const AssetIndex& InAssetIndex, ContentBrowserWindow::DirectoryTreeNode* Root) {
  const StringView Path = InAssetIndex.Path;
  const auto PathSplit = StringUtils::Split(Path, '/');
  ContentBrowserWindow::DirectoryTreeNode* Current = Root;
  for (Int32 Index = 0; Index < PathSplit.Count() - 1; Index++) {
    auto Split = PathSplit[Index];
    ContentBrowserWindow::DirectoryTreeNode* Founded = nullptr;
    for (auto& Value : Current->Children) {
      if (Value->Name == Split) {
        Founded = Value.Get();
        break;
      }
    }
    if (Founded == nullptr) {
      UniquePtr<ContentBrowserWindow::DirectoryTreeNode> New = MakeUnique<ContentBrowserWindow::DirectoryTreeNode>();
      New->Name = Split.ToString();
      New->Indent = static_cast<Int8>(Index);
      New->IsOpened = false;
      Founded = New.Get();
      Current->Children.Add(std::move(New));
    }
    Current = Founded;
  }
  ContentBrowserWindow::FileEntry FileEntry;
  FileEntry.Name = PathSplit.Last().ToString();
  FileEntry.ObjectHandle = InAssetIndex.ObjectHandle;
  FileEntry.Type = InAssetIndex.Type;
  Current->Files.Add(std::move(FileEntry));
}

static UniquePtr<ContentBrowserWindow::DirectoryTreeNode> BuildAssetTree() {
  Array<AssetIndex> Index = AssetsManager::GetRef().internalGetAllAssetIndex();
  UniquePtr<ContentBrowserWindow::DirectoryTreeNode> Root = MakeUnique<ContentBrowserWindow::DirectoryTreeNode>();
  for (auto& AssetIndex : Index) {
    BuildAssetTreeNodeFromIndex(AssetIndex, Root.Get());
  }
  return Root;
}

ContentBrowserWindow::ContentBrowserWindow() {
  mWindowTitle = "内容浏览器";
  mName = "ContentBrowserWindow";
  mShouldDeleteWhenUnVisible = false;
  mAssetTreeRoot = BuildAssetTree();
}

ContentBrowserWindow::~ContentBrowserWindow() {}

static void DrawDirectoryTreeRecursive(ContentBrowserWindow::DirectoryTreeNode* Root) {
  if (Root != nullptr) {
    if (Root->Name.Empty()) {
      for (const auto& Node : Root->Children) {
        DrawDirectoryTreeRecursive(Node.Get());
      }
    } else {
      const String ID = Format("##{}", Root->Name);
      if (ImGui::TreeNode(ID.Data())) {
        ImGui::SameLine();
        const String Text = Format("{} {}", KITA_ICON_FOLDER_OPEN, Root->Name);
        EditorUI::Text(Text.Data());
        for (const auto& TreeNode : Root->Children) {
          DrawDirectoryTreeRecursive(TreeNode.Get());
        }
        for (const auto& File : Root->Files) {
          switch (File.Type) {
          case EAssetType::Texture2D:
            EditorUI::ImageIcon(EditorUI::EEditorImageIcon::Texture);
            break;
          case EAssetType::Mesh:
            EditorUI::ImageIcon(EditorUI::EEditorImageIcon::Mesh);
            break;
          case EAssetType::Shader:
            EditorUI::ImageIcon(EditorUI::EEditorImageIcon::Shader);
            break;
          default:
            EditorUI::ImageIcon(EditorUI::EEditorImageIcon::UnknownFile);
            break;
          }
          EditorUI::SameLine();
          if (EditorUI::Button(File.Name)) {
            // todo: gSelection
          }
        }
        ImGui::TreePop();
      } else {
        ImGui::SameLine();
        String Text = Format("{} {}", KITA_ICON_FOLDER, Root->Name);
        EditorUI::Text(Text.Data());
      }
    }
  }
}

void ContentBrowserWindow::DrawEditorUI() {
  EditorUI::BeginChild("A", {mLeftPanelWidth, 0});
  DrawDirectoryTreeRecursive(mAssetTreeRoot.Get());
  EditorUI::EndChild();
  EditorUI::SameLine();
  EditorUI::Splitter(mLeftPanelWidth, mRightPanelWidth);
  EditorUI::SameLine();

  EditorUI::BeginChild("B", {mRightPanelWidth, 0});
  EditorUI::Text("Hello World");
  EditorUI::EndChild();
}

ContentBrowserWindow::DirectoryTreeNode::DirectoryTreeNode() {}

ContentBrowserWindow::DirectoryTreeNode::~DirectoryTreeNode() {}