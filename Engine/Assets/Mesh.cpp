//
// Created by kita on 25-9-1.
//

#include "Mesh.h"

#include "Core/Assert.h"
#include "Core/Performance/ProfilerMark.h"
#include "RHI/Buffer.h"
#include "RHI/GfxCommandHelper.h"
#include "RHI/GfxContext.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

Mesh::Mesh() = default;

Mesh::~Mesh() = default;

void Mesh::Load() {
  CPU_PROFILING_SCOPE;
  if (mLoaded)
    return;
  if (mHandle == 0) {
    gLogger.Error(Logcat::Asset, "Mesh的ObjectHandle为0.");
    return;
  }
  if (mPath.Empty()) {
    gLogger.Error(Logcat::Asset, "Mesh的Path为空.");
    return;
  }
  if (!mPath.EndsWith(".fbx") && !mPath.EndsWith(".obj")) {
    gLogger.Error(Logcat::Asset, "Mesh的Path '{}' 不是.fbx或.obj.", mPath);
    return;
  }
  mLoaded = LoadFromPath();
}

void Mesh::Unload() {}

void Mesh::ApplyMeta(const AssetMeta& Meta) {
  const auto& ThisMeshMeta = static_cast<const MeshMeta&>(Meta);
  mPath = ThisMeshMeta.Path;
  mHandle = ThisMeshMeta.ObjectHandle;
  mMeta = ThisMeshMeta;
}

bool Mesh::LoadFromPath() {
  CPU_PROFILING_SCOPE;
  // TODO: 这里Load流程如果要打包就不应该一样 而是直接从二进制文件读取 不Import
#if KITA_EDITOR
  Assimp::Importer Importer;
  const aiScene* Scene = Importer.ReadFile(mPath.Data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
  if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
    gLogger.Error(Logcat::Asset, "Assimp导入失败: {}", Importer.GetErrorString());
    return false;
  }
  Array<Vertex> AllVertices;
  Array<UInt32> AllIndices;
  ProcessNode(Scene->mRootNode, Scene, AllVertices, AllIndices);
  // Vertex
  RHIBufferDesc VertexBufferDesc{};
  UInt64 Size = AllVertices.Count() * sizeof(Vertex);
  VertexBufferDesc.SetMemoryProperty(BMPB_HostVisible | BMPB_HostCoherent).SetSize(Size).SetUsage(BUB_TransferSrc);
  UniquePtr<RHIBuffer> TempVertexBuffer = GfxContext::GetRef().CreateBufferU(VertexBufferDesc);
  VertexBufferDesc.SetMemoryProperty(BMPB_DeviceLocal).SetUsage(BUB_VertexBuffer | BUB_TransferDst);
  mVertexBuffer = GfxContext::GetRef().CreateBufferU(VertexBufferDesc);
  auto VertexHandle = GfxCommandHelper::CopyAsync(TempVertexBuffer.Get(), mVertexBuffer.Get(), Size, 0, 0);

  // Index
  RHIBufferDesc IndexBufferDesc{};
  Size = AllIndices.Count() * sizeof(UInt32);
  IndexBufferDesc.SetMemoryProperty(BMPB_HostVisible | BMPB_HostCoherent).SetSize(Size).SetUsage(BUB_TransferSrc);
  UniquePtr<RHIBuffer> TempIndexBuffer = GfxContext::GetRef().CreateBufferU(IndexBufferDesc);
  IndexBufferDesc.SetMemoryProperty(BMPB_DeviceLocal).SetUsage(BUB_IndexBuffer | BUB_TransferDst);
  mIndexBuffer = GfxContext::GetRef().CreateBufferU(IndexBufferDesc);
  auto IndexHandle = GfxCommandHelper::CopyAsync(TempIndexBuffer.Get(), mIndexBuffer.Get(), Size, 0, 0);
  VertexHandle.WaitAll();
  IndexHandle.WaitAll();
  return true;
#endif
}

void Mesh::ProcessMesh(const aiMesh* Mesh, Array<Vertex>& AllVertices, Array<UInt32>& AllIndices) {
  CPU_PROFILING_SCOPE;
  SubmeshInfo Info{};
  Info.VertexOffset = AllVertices.Count();
  Info.VertexCount = Mesh->mNumVertices;
  Info.IndexOffset = AllIndices.Count();
  Info.IndexCount = Mesh->mNumFaces * 3;
  // 处理顶点
  AllVertices.Reserve(AllVertices.Count() + Info.VertexCount);
  for (UInt32 i = 0; i < Mesh->mNumVertices; i++) {
    Vertex Vertex;
    Vertex.Position = Vector3f(Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z);
    Vertex.Normal = Mesh->HasNormals() ? Vector3f(Mesh->mNormals[i].x, Mesh->mNormals[i].y, Mesh->mNormals[i].z) : Vector3f(0.0f);
    if (Mesh->HasTextureCoords(0)) {
      Vertex.UV0 = Vector2f(Mesh->mTextureCoords[0][i].x, Mesh->mTextureCoords[0][i].y);
    } else {
      Vertex.UV0 = Vector2f(0.0f);
    }
    AllVertices.Add(Vertex);
  }
  // 处理索引
  AllIndices.Reserve(AllIndices.Count() + Info.IndexCount);
  for (UInt32 i = 0; i < Mesh->mNumFaces; i++) {
    const aiFace Face = Mesh->mFaces[i];
    for (UInt32 j = 0; j < Face.mNumIndices; j++) {
      AllIndices.Add(Face.mIndices[j]);
    }
  }
  mSubmeshes.Add(Info);
}

void Mesh::ProcessNode(const struct aiNode* Node, const struct aiScene* Scene, Array<Vertex>& AllVertices, Array<UInt32>& AllIndices) {
  for (UInt32 i = 0; i < Node->mNumMeshes; i++) {
    const aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
    ProcessMesh(Mesh, AllVertices, AllIndices);
  }
  for (UInt32 i = 0; i < Node->mNumChildren; i++) {
    ProcessNode(Node->mChildren[i], Scene, AllVertices, AllIndices);
  }
}