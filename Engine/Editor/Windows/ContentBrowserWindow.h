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
    Int8 Indent;
    String Name;
  };

private:
  float mLeftPanelWidth = 100.f;
  float mRightPanelWidth = 0.f;
  DirectoryTreeNode* Root;
};

KCLASS(MenuActionItem = "窗口/内容浏览器")
class MenuAction_OpenContentWindow : public MenuAction {
  GENERATED_BODY(MenuAction_OpenContentWindow)
public:
  virtual void Execute() override { EditorWindowManager::Open<ContentBrowserWindow>(); }
};