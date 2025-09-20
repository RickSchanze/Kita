//
// Created by kita on 25-9-20.
//

#include "TextureInspector.h"

#include "Assets/Texture.h"
#include "Editor/EditorUI.h"
#include "RHI/GfxContext.h"

TextureInspector::ScopedImGuiTexture::~ScopedImGuiTexture() {
  if (ImGuiTexture != nullptr) {
    GfxContext::GetRef().DestroyImGuiTexture(ImGuiTexture);
    ImGuiTexture = nullptr;
  }
}

TextureInspector::TextureInspector() {}

void TextureInspector::DrawGUI() {
  if (mInspectedObject == nullptr) {
    return;
  }
  const auto Texture = static_cast<Texture2D*>(mInspectedObject);
  Header(Texture->GetPath(), Texture->GetHandle());
  Property("宽: ", Texture->GetWidth());
  Property("高: ", Texture->GetHeight());

  const auto ImageWidth = Texture->GetWidth();
  const auto ImageHeight = Texture->GetHeight();
  const auto AvailWidth = EditorUI::GetContentRegionAvail().X();
  Vector2f DrawSize;
  if (AvailWidth >= ImageWidth) {
    const float OffsetX = (AvailWidth - ImageWidth) / 2.0f;
    EditorUI::SetCursorPosX(OffsetX);
    DrawSize = Vector2f(ImageWidth, ImageHeight);
  } else {
    const float Scale = AvailWidth / ImageWidth;
    DrawSize = Vector2f(AvailWidth, ImageHeight * Scale);
  }
  EditorUI::Image(mScopedImGuiTexture.ImGuiTexture, DrawSize);
}

void TextureInspector::OnSetInspectedObject(Object* NewObj) {
  if (NewObj == nullptr) {
    mScopedImGuiTexture = {};
    return;
  }
  const auto Texture = static_cast<Texture2D*>(NewObj);
  void* ImTex = GfxContext::GetRef().CreateImGuiTexture(EditorUI::GetEditorUsedSampler(), Texture->GetImageView());
  mScopedImGuiTexture = ScopedImGuiTexture{ImTex};
}