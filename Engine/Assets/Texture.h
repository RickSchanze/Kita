#pragma once
#include "Asset.h"

#include "RHI/RHIEnums.h"
#include "Texture.generated.h"

class RHIImageView;
class RHIImage;

KSTRUCT()
struct Texture2DMeta : AssetMeta {
  GENERATED_BODY(Texture2DMeta)

  KPROPERTY()
  Int32 Id = 0;

  KPROPERTY()
  String Path;

  KPROPERTY()
  Int32 ObjectHandle = 0;

  KPROPERTY()
  bool EnableMipMap = false;

  KPROPERTY()
  ERHIFormat Format = ERHIFormat::R8G8B8A8_SRGB;
};

KSTRUCT()
struct TextureCubeMeta : AssetMeta {
  GENERATED_BODY(TextureCubeMeta)

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
  Texture();
  virtual ~Texture() override;

  [[nodiscard]] UInt32 GetWidth() const;
  [[nodiscard]] UInt32 GetHeight() const;
  [[nodiscard]] ERHIFormat GetFormat() const;

protected:
  UniquePtr<RHIImage> mImage;
  Texture2DMeta mMeta{};
};

KCLASS()
class Texture2D : public Texture {
  GENERATED_BODY(Texture2D)
public:
  Texture2D();
  virtual ~Texture2D() override;

  virtual EAssetType GetAssetType() override { return EAssetType::Texture2D; }

  virtual void ApplyMeta(const AssetMeta& Meta) override;

  virtual void Load() override;
  virtual void Unload() override;

  RHIImageView* GetImageView() const { return mImageView.Get(); }
  void* GetImGuiTexture() const { return mImGuiTexture; }
private:
  UniquePtr<RHIImageView> mImageView;

#if KITA_EDITOR
  void* mImGuiTexture;
#endif
};

KCLASS()
class TextureCube : public Texture {
  GENERATED_BODY(TextureCube)
public:
  virtual EAssetType GetAssetType() override { return EAssetType::TextureCube; }

  virtual void Load() override;
  virtual void Unload() override;

  virtual void ApplyMeta(const AssetMeta& Meta) override;

private:
};
