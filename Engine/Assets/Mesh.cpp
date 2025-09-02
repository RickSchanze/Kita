//
// Created by kita on 25-9-1.
//

#include "Mesh.h"
#include "RHI/Buffer.h"

Mesh::Mesh() = default;

Mesh::~Mesh() = default;

void Mesh::Load() {

}

void Mesh::Unload() {}

void Mesh::ApplyMeta(const AssetMeta& Meta) {
  const auto& ThisMeshMeta = static_cast<const MeshMeta&>(Meta);
  mPath = ThisMeshMeta.Path;
  mHandle = ThisMeshMeta.ObjectId;
}