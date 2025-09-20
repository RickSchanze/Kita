//
// Created by kita on 25-9-20.
//

#include "MeshInspector.h"

#include "Assets/Mesh.h"

void MeshInspector::DrawGUI() {
  if (mInspectedObject == nullptr)
    return;
  const Mesh* MyMesh = static_cast<Mesh*>(mInspectedObject);
  Header(MyMesh->GetPath(), MyMesh->GetHandle());
  Property("顶点数: ", MyMesh->GetVertexCount());
  Property("索引数: ", MyMesh->GetIndexCount());
  Property("子Mesh数: ", MyMesh->GetSubmeshes().Count());
}
