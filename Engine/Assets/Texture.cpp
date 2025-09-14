//
// Created by kita on 25-9-14.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

UInt32 Texture::GetWidth() const {
  if (!mImage) {
    gLogger.Error("Texture", "Image is null");
    return 0;
  }
  return mImage->GetDesc().Width;
}

UInt32 Texture::GetHeight() const {
  if (!mImage) {
    gLogger.Error("Texture", "Image is null");
    return 0;
  }
  return mImage->GetDesc().Height;
}

ERHIFormat Texture::GetFormat() const {
  if (!mImage) {
    gLogger.Error("Texture", "Image is null");
    return ERHIFormat::Count;
  }
  return mImage->GetDesc().Format;
}
void Texture::ApplyMeta(const AssetMeta& Meta) {
  const auto MyTextureMeta = static_cast<const TextureMeta&>(Meta);
  mMeta = MyTextureMeta;
}

void Texture2D::Load() {}
void Texture2D::Unload() {}
