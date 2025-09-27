#pragma once
#include "Asset.h"
#include "Core/Reflection/MetaMark.h"
#include "Object/ObjectPtr.h"

#include "Material.generated.h"

class Texture2D;
class Shader;
class SharedMaterial {};

KSTRUCT()
struct MaterialMeta : AssetMeta {
  GENERATED_BODY(MaterialMeta)

  KPROPERTY()
  Int32 Id = 0;

  /// 资产路径
  KPROPERTY()
  String Path;

  /// 对象Handle
  KPROPERTY()
  Int32 ObjectHandle = 0;
};

KCLASS()
class Material : public Asset {
  GENERATED_BODY(Material)
public:
  virtual EAssetType GetAssetType() override { return EAssetType::Material; }
  virtual void Load() override;
  virtual void Unload() override;
  virtual void Save() override;
  virtual void ApplyMeta(const AssetMeta& Meta) override;

protected:
  KPROPERTY()
  ObjectPtr<Shader> mShader;

  KPROPERTY()
  Map<String, ObjectPtr<Texture2D>> mTextures;

  MaterialMeta mMeta;
};
