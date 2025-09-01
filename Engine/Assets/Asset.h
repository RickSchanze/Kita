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

KCLASS(Abstract)
class Asset : public PersistentObject {
  GENERATED_BODY(Asset)
public:
  virtual EAssetType GetAssetType() = 0;
};
