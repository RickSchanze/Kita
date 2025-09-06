#pragma once
#include "Core/Config/IConfig.h"

#include "AssetsConfig.generated.h"

KCLASS(Category = Assets)
class AssetsConfig : public IConfig {
  GENERATED_BODY(AssetsConfig)
public:
  KITA_CONFIG_GETTER_SETTER(Array<String>, ShaderSearchPaths)

  void AddShaderSearchPath(const String& NewPath);

private:
  KPROPERTY()
  Array<String> ShaderSearchPaths = {"Assets/Shader"};
};
