#pragma once
#include "Object/Object.h"

#include "MenuAction.generated.h"

// 使用属性MenuActionItem注册一个MenuItem
// KCLASS(MenuActionItem = "工具/日志")
KCLASS(Abstract)
class MenuAction : public Object {
  GENERATED_BODY(MenuAction)
public:
  virtual void Execute() = 0;
};
