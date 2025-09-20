//
// Created by kita on 25-9-13.
//

#include "ShaderInspector.h"

#include "Assets/Shader.h"
#include "Editor/EditorUI.h"

void ShaderInspector::DrawGUI() {
  if (mInspectedObject == nullptr)
    return;
  const Shader* MyShader = static_cast<Shader*>(mInspectedObject);
  Header(MyShader->GetPath(), MyShader->GetHandle());
}
