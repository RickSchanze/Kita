#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Inspector.h"

#include "TextureInspector.generated.h"

KCLASS(Inspector = Texture2D)
class TextureInspector : public Inspector {
  GENERATED_BODY(TextureInspector)

public:
  TextureInspector();
  virtual void DrawGUI() override;
};
