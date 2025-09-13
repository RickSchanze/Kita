#pragma once
#include "Inspector.h"

#include "ShaderInspector.generated.h"

KCLASS(Inspector)
class ShaderInspector : public Inspector
{
  GENERATED_BODY(ShaderInspector)
public:
  virtual const Type* GetInspectedType() override;

  virtual void DrawGUI() override;
};
