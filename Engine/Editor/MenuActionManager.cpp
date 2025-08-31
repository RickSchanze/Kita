//
// Created by kita on 25-8-31.
//

#include "MenuActionManager.h"

#include "Core/Performance/ProfilerMark.h"
#include "Core/Reflection/ReflUtils.h"
#include "Core/Reflection/TypeRegistry.h"
// #include "Core/String/StringUtils.h"
#include "Core/String/StringUtils.h"
#include "EditorUI.h"
#include "MenuAction.h"

MenuItemTreeNode::~MenuItemTreeNode() {
  for (auto* Child : Children) {
    Delete(Child);
  }
}

void MenuActionManager::StartUp() {
  auto& Self = GetRef();
  Self.mRoot = New<MenuItemTreeNode>();
  // 遍历所有Type 找到所有带有MenuAction的类
  const Type* MenuActionType = TypeOf<MenuAction>();
  auto Types = ReflUtils::GetTypes([MenuActionType](const Type* InType) {
    if (InType->IsDerivedFrom(MenuActionType)) {
      if (auto MenuActionAttribute = InType->GetAttribute("MenuActionItem")) {
        if (!MenuActionAttribute->Empty()) {
          return true;
        }
      }
    }
    return false;
  });
  for (const auto* Type : Types) {
    Self.RegisterMenuActionItem(Type);
  }
}

void MenuActionManager::ShutDown() { Delete(GetRef().mRoot); }

void MenuActionManager::Render() {
  CPU_PROFILING_SCOPE;
  EditorUI::BeginMenuBar();
  DrawMenuRecursive(GetRef().mRoot);
  EditorUI::EndMenuBar();
}

void MenuActionManager::RegisterMenuActionItem(const Type* InType) const {
  const auto OpMenuActionItemPath = InType->GetAttribute("MenuActionItem");
  if (!OpMenuActionItemPath) {
    return;
  }
  StringView MenuActionItemPath = *OpMenuActionItemPath;
  Array<StringView> Paths = StringUtils::Split(MenuActionItemPath, '/');
  MenuItemTreeNode* CurrentNode = mRoot;
  for (auto Path : Paths) {
    if (CurrentNode->Children.Empty()) {
      MenuItemTreeNode* NewNode = New<MenuItemTreeNode>();
      NewNode->Path = Path.ToString();
      CurrentNode->Children.Add(NewNode);
      CurrentNode = NewNode;
      continue;
    }
    for (const auto Child : CurrentNode->Children) {
      if (Child->Path == Path) {
        CurrentNode = Child;
        continue;
      }
      MenuItemTreeNode* NewNode = New<MenuItemTreeNode>();
      NewNode->Path = Path.ToString();
      CurrentNode->Children.Add(NewNode);
      CurrentNode = NewNode;
    }
  }
  CurrentNode->Action = InType->CreateInstanceT<MenuAction>();
}

void MenuActionManager::DrawMenuRecursive(MenuItemTreeNode* Root) {
  for (auto* Child : Root->Children) {
    if (Child->Children.Count() == 0) { // 叶节点则绘制Action
      if (EditorUI::MenuItem(Child->Path)) {
        Child->Action->Execute();
      }
    } else {
      // 非叶节点绘制下一集菜单
      if (EditorUI::BeginMenu(Child->Path)) {
        DrawMenuRecursive(Child);
        EditorUI::EndMenu();
      }
    }
  }
}