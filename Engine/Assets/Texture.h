#pragma once
#include "Asset.h"
#include "RHI/Image.h"

#include "Texture.generated.h"

class RHIImageView;
class RHIImage;
KSTRUCT()
struct TextureMeta : AssetMeta {
  GENERATED_BODY(TextureMeta)

  KPROPERTY()
  Int32 Id = 0;

  KPROPERTY()
  String Path;

  KPROPERTY()
  Int32 ObjectHandle = 0;

  KPROPERTY()
  bool EnableMipMap = false;
};

KCLASS(Abstract)
class Texture : public Asset {
  GENERATED_BODY(Texture)
public:
  [[nodiscard]] UInt32 GetWidth() const;
  [[nodiscard]] UInt32 GetHeight() const;
  [[nodiscard]] ERHIFormat GetFormat() const;
  virtual void ApplyMeta(const AssetMeta& Meta) override;

private:
  UniquePtr<RHIImage> mImage;
  TextureMeta mMeta;
};

KCLASS()
class Texture2D : public Texture {
  GENERATED_BODY(Texture2D)
public:
  virtual EAssetType GetAssetType() override { return EAssetType::Texture2D; }

  virtual void Load() override;
  virtual void Unload() override;

private:
  UniquePtr<RHIImageView> mImageView;
};

KCLASS()
class TextureCube : public Texture {
  GENERATED_BODY(TextureCube)
public:
  virtual EAssetType GetAssetType() override { return EAssetType::TextureCube; }

  virtual void Load() override;
  virtual void Unload() override;
private:
};
