//
// Created by kita on 25-9-5.
//

#include "Shader.h"

void Shader::Load() {
  if (mPath.Empty() || !mPath.EndsWith(".slang")) {
    gLogger.Error(Logcat::Asset, "无效的Shader路径.");
    return;
  }

}

void Shader::Unload() {

}
void Shader::ApplyMeta(const AssetMeta& Meta) {
  const auto& MyShaderMeta = static_cast<const ShaderMeta&>(Meta);
  mPath = MyShaderMeta.Path;
  // Apply时Handle已经处理
}

void Shader::ReadCache() {}

void Shader::WriteCache() {

}