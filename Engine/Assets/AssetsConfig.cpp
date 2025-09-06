//
// Created by kita on 25-9-6.
//

#include "AssetsConfig.h"
void AssetsConfig::AddShaderSearchPath(StringView NewPath) {
  if (ShaderSearchPaths.IndexOf(NewPath) == INVALID_INDEX) {
    ShaderSearchPaths.Add(NewPath);
#if KITA_EDITOR
    mSaveDirty = false;
#endif
  }
}