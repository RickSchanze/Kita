#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Inspector.h"

#include "TextureInspector.generated.h"

KCLASS(Inspector = Texture2D)
class TextureInspector : public Inspector {
  GENERATED_BODY(TextureInspector)

  struct ScopedImGuiTexture : NonCopyable {
    void* ImGuiTexture = nullptr;

    ScopedImGuiTexture(void* Tex = nullptr) : ImGuiTexture(Tex) {}

    ~ScopedImGuiTexture();

    ScopedImGuiTexture(ScopedImGuiTexture&& Other) noexcept {
      ImGuiTexture = Other.ImGuiTexture;
      Other.ImGuiTexture = nullptr;
    }
    ScopedImGuiTexture& operator=(ScopedImGuiTexture&& Other) noexcept {
      ImGuiTexture = Other.ImGuiTexture;
      Other.ImGuiTexture = nullptr;
      return *this;
    }
  };

public:
  TextureInspector();
  virtual void DrawGUI() override;
  virtual void OnSetInspectedObject(Object* NewObj) override;

private:
  ScopedImGuiTexture mScopedImGuiTexture{};
};
