#pragma once
#include "Object/PersistentObject.h"

#include "Asset.generated.h"

namespace Logcat {
inline auto Asset = "Asset";
}

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

  /// 数据库主键Id
  KPROPERTY()
  Int32 Id = 0;

  /// 资产路径
  KPROPERTY()
  String Path;

  /// 对象Handle
  KPROPERTY()
  Int32 ObjectHandle = 0;
};

KCLASS(Abstract)
class Asset : public PersistentObject {
  GENERATED_BODY(Asset)
public:
  virtual EAssetType GetAssetType() = 0;

  /// 传入自己资产对应的Meta文件, 然后转化为自己的一些属性
  virtual void ApplyMeta(const AssetMeta& Meta) = 0;
};
