#pragma once
#include "Core/Reflection/MetaMark.h"

#include "Inspector.generated.h"

struct Type;

KCLASS(Abstract)
class Inspector {
  GENERATED_BODY(Inspector)
public:
  virtual ~Inspector() = default;

  // 获取正在查看的类型
  virtual const Type* GetInspectedType() = 0;

  virtual void DrawGUI() = 0;
};
