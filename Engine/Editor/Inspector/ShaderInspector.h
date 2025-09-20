#pragma once
#include "Inspector.h"

#include "ShaderInspector.generated.h"

KCLASS(Inspector=Shader)
class ShaderInspector : public Inspector
{
  GENERATED_BODY(ShaderInspector)
public:
  virtual void DrawGUI() override;
};
