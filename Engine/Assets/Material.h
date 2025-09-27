#pragma once
#include "Asset.h"
#include "Core/Reflection/MetaMark.h"
#include "Core/Singleton/Singleton.h"
#include "Object/ObjectPtr.h"

#include "Material.generated.h"

class RHIPipeline;
class RHIPipelineLayout;
class Texture2D;
class Shader;

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

class SharedMaterialImpl {
public:
  SharedMaterialImpl();
  ~SharedMaterialImpl();

private:
  UniquePtr<RHIPipelineLayout> mPipelineLayout;
  UniquePtr<RHIPipeline> mPipeline;
};

class SharedMaterial {
public:
  explicit SharedMaterial(const SharedPtr<SharedMaterialImpl>& Mat) : mImpl(Mat) {}
  SharedMaterial(const SharedMaterial& Other) : mImpl(Other.mImpl) {}

  // 通知SharedMaterialManager SharedMaterial被销毁
  ~SharedMaterial();

  [[nodiscard]] bool IsValid() const { return mImpl != nullptr; }

private:
  SharedPtr<SharedMaterialImpl> mImpl = nullptr;
};

class SharedMaterialManager : Singleton<SharedMaterialManager> {
public:
  // 创建一个SharedMaterial
  static SharedMaterial Create(Shader* InShader);
  // 获取一个SharedMaterial
  static SharedMaterial Get(const Shader* InShader);
  // 获取一个SharedMaterial 如果不存在则创建一个
  static SharedMaterial Acquire(Shader* InShader);

private:
  Map<UInt64, SharedPtr<SharedMaterialImpl>> mSharedMaterials;
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
