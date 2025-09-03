#pragma once
#include "Asset.h"
#include "Core/Memory/UniquePtr.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/Singleton/Singleton.h"
#include "Core/TaskGraph/TaskHandle.h"
#include "Mesh.h"

struct AssetIndex {
  Int32 Id;
  Int32 ObjectHandle;
  std::string Name;
  std::string Path;
  EAssetType Type;
};

struct AssetLoadTaskHandle : TaskHandle {
  explicit AssetLoadTaskHandle(const TaskHandle& InHandle) : TaskHandle(InHandle) {}

  Asset* GetAssetObject() {
    CPU_PROFILING_SCOPE;
    WaitSync();
    return mTargetObject;
  }

  template <typename T> T* GetAssetObjectT() {
    CPU_PROFILING_SCOPE;
    WaitSync();
    return static_cast<T*>(mTargetObject);
  }

private:
  Asset* mTargetObject;
};

template <typename T> struct AssetMetaType;
template <> struct AssetMetaType<Mesh> {
  using Type = MeshMeta;
};

class AssetsManager : public Singleton<AssetsManager> {
public:
  AssetsManager();
  ~AssetsManager();
  /// 此函数会预加载一些引擎一定会用到的资产
  static void StartUp();

  /// 释放所有资产
  static void ShutDown();

  AssetLoadTaskHandle LoadAsync(StringView Path);
  AssetLoadTaskHandle LoadAsync(Int32 ObjectHandle);

  Optional<MeshMeta> QueryMeshMeta(StringView Path);
  Optional<MeshMeta> QueryMeshMeta(Int32 ObjectHandle);

  template <typename T> Optional<AssetMetaType<T>> QueryMeta(StringView Path);

  template <typename T> Optional<AssetMetaType<T>> QueryMeta(Int32 ObjectHandle);

private:
  struct Impl;
  UniquePtr<Impl> mImpl;
};

#pragma region 模版实现

template <typename T> Optional<AssetMetaType<T>> AssetsManager::QueryMeta(const StringView Path) {
  if constexpr (Traits::SameAs<T, Mesh>) {
    return QueryMeshMeta(Path);
  } else {
    static_assert(false, "Unsupported Asset Type");
  }
  return {};
}

template <typename T> Optional<AssetMetaType<T>> AssetsManager::QueryMeta(const Int32 ObjectHandle) {
  if constexpr (Traits::SameAs<T, Mesh>) {
    return QueryMeshMeta(ObjectHandle);
  } else {
    static_assert(false, "Unsupported Asset Type");
  }
  return {};
}

#pragma endregion