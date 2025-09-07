#pragma once
#include "Asset.h"
#include "Core/Memory/UniquePtr.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/Singleton/Singleton.h"
#include "Core/TaskGraph/TaskHandle.h"
#include "Mesh.h"

struct ShaderMeta;

struct AssetIndex {
  Int32 Id{};
  Int32 ObjectHandle{};
  String Path{};
  EAssetType Type{EAssetType::Count};
};

struct AssetLoadTaskHandle : TaskHandle {
  AssetLoadTaskHandle() = default;
  explicit AssetLoadTaskHandle(const TaskHandle& InHandle, Asset* Asset) : TaskHandle(InHandle), mTargetObject(Asset) {}

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

  void internalSetTargetObject(Asset* Asset) { mTargetObject = Asset; }

private:
  Asset* mTargetObject = nullptr;
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

  static AssetLoadTaskHandle LoadAsync(const StringView Path) { return GetRef().LoadAsyncM(Path); }
  AssetLoadTaskHandle LoadAsyncM(StringView Path);

  static AssetLoadTaskHandle LoadAsync(const Int32 ObjectHandle) { return GetRef().LoadAsyncM(ObjectHandle); }
  AssetLoadTaskHandle LoadAsyncM(Int32 ObjectHandle);

  /// 根据路径自动导入资产, 如果资产已经存在则Warn
  /// 导入后会进行加载 返回一个任务句柄
  static AssetLoadTaskHandle ImportAsync(const StringView Path) { return GetRef().ImportAsyncM(Path); }
  AssetLoadTaskHandle ImportAsyncM(StringView Path);

  Optional<MeshMeta> QueryMeshMeta(StringView Path);
  Optional<MeshMeta> QueryMeshMeta(Int32 ObjectHandle);

  static bool IsAssetLoading(Int32 Handle);
  static bool IsAssetLoaded(Int32 Handle);

  template <typename T> Optional<AssetMetaType<T>> QueryMeta(StringView Path);

  template <typename T> Optional<AssetMetaType<T>> QueryMeta(Int32 ObjectHandle);

  struct Impl;

private:
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