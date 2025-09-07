#pragma once
#include "Asset.h"

#include "Math/Vector.h"
#include "Mesh.generated.h"

class RHIBuffer;

KSTRUCT()
struct MeshMeta : AssetMeta {
  GENERATED_BODY(MeshMeta)
  /// 数据库主键Id
  KPROPERTY()
  Int32 Id = 0;

  /// 资产路径
  KPROPERTY()
  String Path;

  /// 对象Handle
  KPROPERTY()
  Int32 ObjectHandle = 0;

  KPROPERTY()
  bool FlipUV = true;

  KPROPERTY()
  bool CalcTangent = true;

  /// 保证所有的面都是三角形
  KPROPERTY()
  bool EnsureTriangles = true;
};

struct Vertex {
  Vector3f Position;
  Vector3f Normal;
  Vector2f UV0;
};

KSTRUCT()
struct SubMeshInfo {
  GENERATED_BODY(SubMeshInfo)

  KPROPERTY(Transient, Label = "顶点偏移")
  UInt32 VertexOffset;

  KPROPERTY(Transient, Label = "顶点数")
  UInt32 VertexCount;

  KPROPERTY(Transient, Label = "索引偏移")
  UInt32 IndexOffset;

  KPROPERTY(Transient, Label = "索引数")
  UInt32 IndexCount;
};

KCLASS()
class Mesh : public Asset {
  GENERATED_BODY(Mesh)
public:
  Mesh();
  virtual ~Mesh() override;

  virtual EAssetType GetAssetType() override { return EAssetType::Mesh; }

  virtual void Load() override;

  virtual void Unload() override;

  virtual void ApplyMeta(const AssetMeta& Meta) override;

  [[nodiscard]] const Array<SubMeshInfo>& GetSubmeshes() const { return mSubMeshes; }
  [[nodiscard]] RHIBuffer* GetVertexBuffer() const { return mVertexBuffer.Get(); }
  [[nodiscard]] RHIBuffer* GetIndexBuffer() const { return mIndexBuffer.Get(); }
  [[nodiscard]] Int32 GetVertexCount() const { return mVertexCount; }
  [[nodiscard]] Int32 GetIndexCount() const { return mIndexCount; }
  [[nodiscard]] SubMeshInfo GetSubmeshInfo(const Int32 Index) const { return mSubMeshes[Index]; }

private:
  bool LoadFromPath();

  void ProcessMesh(const struct aiMesh* Mesh, Array<Vertex>& AllVertices, Array<UInt32>& AllIndices);
  void ProcessNode(const struct aiNode* Node, const struct aiScene* Scene, Array<Vertex>& AllVertices, Array<UInt32>& AllIndices);

  KPROPERTY(Transient, Label = "总顶点数")
  Int32 mVertexCount = 0;

  KPROPERTY(Transient, Label = "总索引数")
  Int32 mIndexCount = 0;

  KPROPERTY(Transient, Label = "子Mesh")
  Array<SubMeshInfo> mSubMeshes;

  KPROPERTY(Transient, Label = "导入选项")
  MeshMeta mMeta;

  UniquePtr<RHIBuffer> mVertexBuffer;
  UniquePtr<RHIBuffer> mIndexBuffer;
};
