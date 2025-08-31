#pragma once
#include "Core/Container/Map.h"
#include "Core/Singleton/Singleton.h"

// 防止编译期优化
#include "Windows/LoggingWindow.h"

struct Type;
class MenuAction;

struct MenuItemTreeNode {
  String Path;
  MenuAction* Action;
  Array<MenuItemTreeNode*> Children;

  ~MenuItemTreeNode();
};

class MenuActionManager : public Singleton<MenuActionManager> {
public:
  static void StartUp();
  static void ShutDown();

  static void Render();

private:
  void RegisterMenuActionItem(const Type* InType) const;

  static void DrawMenuRecursive(MenuItemTreeNode* Root);

  MenuItemTreeNode* mRoot = nullptr;
};
