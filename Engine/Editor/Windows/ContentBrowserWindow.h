#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/MenuAction.h"

#include "Assets/Asset.h"
#include "ContentBrowserWindow.generated.h"

KCLASS()
class ContentBrowserWindow : public EditorWindow {
  GENERATED_BODY(ContentBrowserWindow)
public:
  ContentBrowserWindow();
  virtual ~ContentBrowserWindow() override;
  virtual void DrawEditorUI() override;

  struct FileEntry {
    String Name;
    Int32 ObjectHandle{0};
    EAssetType Type{EAssetType::Count};
  };

  struct DirectoryTreeNode {
    DirectoryTreeNode();
    ~DirectoryTreeNode();

    Array<UniquePtr<DirectoryTreeNode>> Children{};
    DirectoryTreeNode* Parent{};
    Array<FileEntry> Files{};
    bool IsOpened{false};
    Int8 Indent{};
    String Name{};
  };

private:
  void SetCurrentSelectedFolder(DirectoryTreeNode* Node);
  void DrawDirectoryContent(DirectoryTreeNode* Node);
  float mLeftPanelWidth = 100.f;
  float mRightPanelWidth = 0.f;
  UniquePtr<DirectoryTreeNode> mAssetTreeRoot{};
  DirectoryTreeNode* mCurrentSelectedTreeNode = nullptr;
};

KCLASS(MenuActionItem = "窗口/内容浏览器")
class MenuAction_OpenContentWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenContentWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<ContentBrowserWindow>(); }
};