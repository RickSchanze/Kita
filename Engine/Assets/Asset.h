#pragma once
#include "Object/PersistentObject.h"

#include "Asset.generated.h"

namespace Logcat {
inline auto Asset = "Asset";
} // namespace Logcat

/**
 * 只是为了ApplyMeta可以多态而设置
 * 因为如果这里有成员, 那么注册sqlite orm时的类型就是AssetMeta而不是具体的类型
 * 例如注册MeshMeta时, 如果这里有成员 那么注册的类型就是AssetMeta而不是MeshMeta
 */
struct AssetMeta {
  typedef AssetMeta ThisStruct;
};

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

KCLASS(Abstract)
class Asset : public PersistentObject {
  GENERATED_BODY(Asset)
public:
  virtual EAssetType GetAssetType() = 0;

  /// 传入自己资产对应的Meta文件, 然后转化为自己的一些属性
  virtual void ApplyMeta(const AssetMeta& Meta) = 0;
};
