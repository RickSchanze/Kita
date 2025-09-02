#pragma once
#include "Object/PersistentObject.h"

#include "Asset.generated.h"

KENUM()
enum class EAssetType {
  Shader,
  Mesh,
  Texture,
  Material,
  Scene,
  Animation,
  Audio,
  Font,
  Prefab,
};

KSTRUCT()
struct AssetMeta {
  GENERATED_BODY(AssetMeta)

  KPROPERTY()
  String Path;

  KPROPERTY()
  Int32 ObjectId = 0;
};

KCLASS(Abstract)
class Asset : public PersistentObject {
  GENERATED_BODY(Asset)
public:
  virtual EAssetType GetAssetType() = 0;

  /// 传入自己资产对应的Meta文件, 然后转化为自己的一些属性
  virtual void ApplyMeta(const AssetMeta& Meta) = 0;
};
