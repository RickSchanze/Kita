#pragma once
#include "Asset.h"

#include "Mesh.generated.h"

class RHIBuffer;
KCLASS()
class Mesh : public Asset {
  GENERATED_BODY(Mesh)
public:
  Mesh();
  virtual ~Mesh() override;

  virtual EAssetType GetAssetType() override { return EAssetType::Mesh; }

  virtual void Load() override;

  virtual void Unload() override;

private:
  KPROPERTY(Transient)
  Int32 mVertexCount = 0;

  KPROPERTY(Transient)
  Int32 mIndexCount = 0;

  UniquePtr<RHIBuffer> mVertexBuffer;
  UniquePtr<RHIBuffer> mIndexBuffer;
};
