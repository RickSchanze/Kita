#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/MenuAction.h"

#include "ContentBrowserWindow.generated.h"

KCLASS()
class ContentBrowserWindow : public EditorWindow {
  GENERATED_BODY(ContentBrowserWindow)
public:
  ContentBrowserWindow();
  virtual ~ContentBrowserWindow() override;
  virtual void DrawEditorUI() override;

  struct DirectoryTreeNode {
    Array<DirectoryTreeNode*> Children;
    bool IsOpened;
  };

private:
  float mLeftPanelWidth = 0.f;
  bool IsFolderOpen = false;
  DirectoryTreeNode* Root;
};

KCLASS(MenuActionItem = "窗口/内容浏览器")
class MenuAction_OpenContentWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenContentWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<ContentBrowserWindow>(); }
};