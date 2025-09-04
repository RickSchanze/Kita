//
// Created by kita on 25-9-3.
//

#include "AssetsManager.h"

#include "Core/Container/Set.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "Core/TaskGraph/TaskNode.h"
#include "Mesh.h"
#include "Object/ObjectTable.h"
#include "Project/Project.h"
#include "sqlite_orm/sqlite_orm.h"

using namespace sqlite_orm;

AssetsManager::AssetsManager() = default;
AssetsManager::~AssetsManager() = default;

void AssetsManager::StartUp() {}

void AssetsManager::ShutDown() {}

constexpr auto ASSET_CENTER_DATABASE_NAME = "AssetCenter.db";

#pragma region AssetType的SqliteOrm适配
static std::string AssetTypeToString(const EAssetType AssetType) {
  switch (AssetType) {
  case EAssetType::Shader:
    return "Shader";
  case EAssetType::Material:
    return "Material";
  case EAssetType::Mesh:
    return "Mesh";
  default:
    return "MAXCOUNT";
  }
}
static std::unique_ptr<EAssetType> AssetTypeFromString(const std::string& s) {
  if (s == "Shader") {
    return std::make_unique<EAssetType>(EAssetType::Shader);
  }
  if (s == "Material") {
    return std::make_unique<EAssetType>(EAssetType::Material);
  }
  return nullptr;
}

namespace sqlite_orm {
template <> struct type_printer<EAssetType> : text_printer {};

template <> struct statement_binder<EAssetType> {
  static int bind(sqlite3_stmt* stmt, const int index, const EAssetType& value) { return statement_binder<std::string>().bind(stmt, index, AssetTypeToString(value)); }
};

template <> struct field_printer<EAssetType> {
  std::string operator()(const EAssetType& t) const { return AssetTypeToString(t); }
};

template <> struct row_extractor<EAssetType> {
  static EAssetType extract(const char* ColumnText) {
    if (const auto Gender = AssetTypeFromString(ColumnText)) {
      return *Gender;
    } else {
      throw std::runtime_error("incorrect gender string (" + std::string(ColumnText) + ")");
    }
  }

  static EAssetType extract(sqlite3_stmt* Stmt, const int ColumnIndex) {
    const auto Str = sqlite3_column_text(Stmt, ColumnIndex);
    return sqlite_orm::row_extractor<EAssetType, void>::extract(reinterpret_cast<const char*>(Str));
  }
};
} // namespace sqlite_orm

#pragma endregion

#pragma region String的SqliteOrm适配
namespace sqlite_orm {
template <> struct type_printer<String> : public text_printer {};

template <> struct statement_binder<String> {
  static int bind(sqlite3_stmt* stmt, const int index, const String& value) { return statement_binder<std::string>().bind(stmt, index, value.GetStdString()); }
};

template <> struct field_printer<String> {
  std::string operator()(const String& value) const { return "'" + value.GetStdString() + "'"; }
};

template <> struct row_extractor<String> {
  static String extract(const char* row_value) { return {row_value ? row_value : ""}; }

  static String extract(sqlite3_stmt* stmt, const int columnIndex) {
    auto text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, columnIndex));
    return {text ? text : ""};
  }
};
} // namespace sqlite_orm

#pragma endregion

static auto MakeAssetIndexTable() {
  return make_table("AssetIndex", make_column("Id", &AssetIndex::Id, primary_key().autoincrement()), make_column("ObjectId", &AssetIndex::ObjectHandle), make_column("Name", &AssetIndex::Name),
                    make_column("Path", &AssetIndex::Path), make_column("Type", &AssetIndex::Type));
}

static auto MakeMeshMetaTable() {
  return make_table("Mesh", make_column("Id", &MeshMeta::Id, primary_key().autoincrement()), make_column("ObjectId", &MeshMeta::ObjectHandle), make_column("Path", &MeshMeta::Path),
                    make_column("FlipUV", &MeshMeta::FlipUV), make_column("CalcTangent", &MeshMeta::CalcTangent), make_column("EnsureTriangles", &MeshMeta::EnsureTriangles));
}

static auto MakeStorage() {
  const auto LibPath = Project::GetLibraryPath();
  const auto Path = ::Path::Combine(LibPath, ASSET_CENTER_DATABASE_NAME);
  return make_storage(LibPath.Data(), MakeAssetIndexTable(), MakeMeshMetaTable());
}

struct AssetsManager::Impl {
  Impl()
      : mStorage(MakeStorage()) // NOLINT(*-use-equals-default, *-pro-type-member-init)
  {
    mStorage.sync_schema();
  }

  template <typename O, class... Ids> std::unique_ptr<O> GetPointer(Ids&&... InIds) {
    std::lock_guard Lock(mDatabaseMutex);
    return mStorage.get_pointer<O>(std::forward<Ids>(InIds)...);
  }

  template <class T, class... Args> auto GetAll(Args&&... args) {
    std::lock_guard Lock(mDatabaseMutex);
    return mStorage.get_all<T>(std::forward<Args>(args)...);
  }

  template <typename O> int Insert(const O& o) {
    std::lock_guard Lock(mDatabaseMutex);
    const int Cnt = mStorage.insert(o);
    return Cnt;
  }

  template <typename MetaType> Optional<MetaType> QueryMeta(Int32 ObjectHandle) {
    std::lock_guard Lock(mDatabaseMutex);
    const auto Metas = GetAll<MetaType>(where(c(&MetaType::ObjectHandle) == ObjectHandle), limit(1));
    if (Metas.empty()) {
      return {};
    }
    return Metas[0];
  }

  template <typename MetaType> Optional<MetaType> QueryMeta(const StringView Path) {
    std::lock_guard Lock(mDatabaseMutex);
    const auto Metas = GetAll<MetaType>(where(c(&MetaType::Path) == Path.ToString()), limit(1));
    if (Metas.empty()) {
      return {};
    }
    return Metas[0];
  }

  void MakeObjectLoading(const Int32 ObjectHandle) {
    AutoLock Lock(mLoadedAssetsMutex, mLoadingAssetsMutex);
    if (mLoadedAssets.Contains(ObjectHandle)) {
      gLogger.Error(Logcat::Asset, "MakeObjectLoading: 对象 '{}' 已经被加载, 无法被标记为正在加载", ObjectHandle);
      return;
    }
    mLoadingAssets.Add(ObjectHandle);
  }

  void MakeObjectLoaded(const Int32 ObjectHandle) {
    AutoLock Lock(mLoadedAssetsMutex, mLoadingAssetsMutex);
    if (!mLoadedAssets.Contains(ObjectHandle)) {
      gLogger.Error(Logcat::Asset, "MakeObjectLoaded: 对象 '{}' 没有被标记为正在加载, 无法被标记为已加载", ObjectHandle);
      return;
    }
    mLoadedAssets.Add(ObjectHandle);
    mLoadingAssets.Remove(ObjectHandle);
  }

  bool IsLoaded(const Int32 Handle) {
    AutoLock Lock(mLoadedAssetsMutex);
    return mLoadedAssets.Contains(Handle);
  }

  bool IsLoading(const Int32 Handle) {
    AutoLock Lock(mLoadingAssetsMutex);
    return mLoadingAssets.Contains(Handle);
  }

  /// 数据库对象
  DECL_TRACKABLE_MUREX(std::mutex, mDatabaseMutex);
  decltype(MakeStorage()) mStorage;

  /// 当前已经加载完成的资产
  DECL_TRACKABLE_MUREX(std::mutex, mLoadedAssetsMutex);
  Set<Int32> mLoadedAssets;

  /// 当前正在加载的资产
  DECL_TRACKABLE_MUREX(std::mutex, mLoadingAssetsMutex);
  Set<Int32> mLoadingAssets;
};

Optional<MeshMeta> AssetsManager::QueryMeshMeta(const StringView Path) { return mImpl->QueryMeta<MeshMeta>(Path); }

Optional<MeshMeta> AssetsManager::QueryMeshMeta(const Int32 ObjectHandle) { return mImpl->QueryMeta<MeshMeta>(ObjectHandle); }

bool AssetsManager::IsAssetLoading(Int32 Handle) { return GetRef().mImpl->IsLoading(Handle); }

bool AssetsManager::IsAssetLoaded(Int32 Handle) { return GetRef().mImpl->IsLoaded(Handle); }

class LoadMeshTask : public TaskNode {
public:
  LoadMeshTask(Mesh* InMesh) : Mesh(InMesh) {}
  virtual ENamedThread GetDesiredThread() const override { return ENamedThread::IO; }

  virtual ETaskNodeResult Run() override {
    if (Mesh) {
      Mesh->Load();
    }
    return ETaskNodeResult::Success;
  }

  Mesh* Mesh;
};

struct AssetLoadCompletedTask : public TaskNode {
public:
  AssetLoadCompletedTask(AssetsManager::Impl* InImpl, Int32 InObjectHandle) : Impl(InImpl), ObjectHandle(InObjectHandle) {}

  virtual ENamedThread GetDesiredThread() const override { return ENamedThread::IO; }

  virtual ETaskNodeResult Run() override {
    if (Impl && ObjectHandle != 0) {
      Impl->MakeObjectLoaded(ObjectHandle);
    }
    return ETaskNodeResult::Success;
  }

  AssetsManager::Impl* Impl;
  Int32 ObjectHandle;
};

AssetLoadTaskHandle AssetsManager::LoadMeshAsync(const MeshMeta& Meta) {
  CPU_PROFILING_SCOPE;
  Mesh* MyMesh = CreateObject<Mesh>(Meta.Path);
  MyMesh->ApplyMeta(Meta);
  ObjectTable::ModifyObjectHandle(MyMesh, Meta.ObjectHandle);
  mImpl->MakeObjectLoading(Meta.ObjectHandle);
  const TaskHandle Handle = TaskGraph::CreateTask<LoadMeshTask>(Format("Task: Load mesh '{}'", Meta.Path), {}, MyMesh);
  TaskGraph::CreateTask<AssetLoadCompletedTask>("Task: AssetCompleted", {Handle}, mImpl.Get(), Meta.ObjectHandle);
  return AssetLoadTaskHandle(Handle, MyMesh);
}

AssetLoadTaskHandle AssetsManager::LoadAsyncM(const StringView Path) {
  // 首先从AssetIndex里查询
  CPU_PROFILING_SCOPE;
  auto OpAssetIndex = mImpl->QueryMeta<AssetIndex>(Path);
  if (!OpAssetIndex) {
    return {};
  }
  switch (const AssetIndex& AssetIndex = *OpAssetIndex; AssetIndex.Type) {
  case EAssetType::Mesh: {
    auto OpMeshMeta = mImpl->QueryMeta<MeshMeta>(Path);
    if (!OpMeshMeta) {
      return {};
    }
    return LoadMeshAsync(*OpMeshMeta);
  }
  default:
    return {};
  }
}

AssetLoadTaskHandle AssetsManager::LoadAsyncM(const Int32 ObjectHandle) {
  // 首先从AssetIndex里查询
  CPU_PROFILING_SCOPE;
  auto OpAssetIndex = mImpl->QueryMeta<AssetIndex>(ObjectHandle);
  if (!OpAssetIndex) {
    return {};
  }
  switch (const AssetIndex& AssetIndex = *OpAssetIndex; AssetIndex.Type) {
  case EAssetType::Mesh: {
    auto OpMeshMeta = mImpl->QueryMeta<MeshMeta>(ObjectHandle);
    if (!OpMeshMeta) {
      return {};
    }
    return LoadMeshAsync(*OpMeshMeta);
  }
  default:
    return {};
  }
}
