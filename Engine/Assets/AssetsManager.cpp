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
#include "Shader.h"
#include "sqlite_orm/sqlite_orm.h"

using namespace sqlite_orm;

constexpr auto ASSET_DATABASE_NAME = "AssetsData.db";

#pragma region Enum的SqliteOrm适配

namespace sqlite_orm {
template <typename T>
  requires std::is_enum_v<T>
struct type_printer<T> : text_printer {};

template <typename T>
  requires std::is_enum_v<T>
struct statement_binder<T> {
  static int bind(sqlite3_stmt* stmt, const int index, const T& value) { return statement_binder<std::string>().bind(stmt, index, EnumToString(value).GetStdString()); }
};

template <typename T>
  requires std::is_enum_v<T>
struct field_printer<T> {
  std::string operator()(const T& t) const { return EnumToString(t).GetStdString(); }
};

template <typename T>
  requires std::is_enum_v<T>
struct row_extractor<T> {
  static T extract(const char* ColumnText) { return StringToEnum<T>(ColumnText); }

  static T extract(sqlite3_stmt* Stmt, const int ColumnIndex) {
    const auto Str = sqlite3_column_text(Stmt, ColumnIndex);
    return sqlite_orm::row_extractor<T, void>::extract(reinterpret_cast<const char*>(Str));
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
  return make_table("AssetIndex", make_column("Id", &AssetIndex::Id, primary_key().autoincrement()), make_column("ObjectHandle", &AssetIndex::ObjectHandle), make_column("Path", &AssetIndex::Path),
                    make_column("Type", &AssetIndex::Type));
}

static auto MakeMeshMetaTable() {
  return make_table("Mesh", make_column("Id", &MeshMeta::Id, primary_key().autoincrement()), make_column("ObjectHandle", &MeshMeta::ObjectHandle), make_column("Path", &MeshMeta::Path),
                    make_column("FlipUV", &MeshMeta::FlipUV), make_column("CalcTangent", &MeshMeta::CalcTangent), make_column("EnsureTriangles", &MeshMeta::EnsureTriangles));
}

static auto MakeShaderMetaTable() {
  return make_table("Shader", make_column("Id", &ShaderMeta::Id, primary_key().autoincrement()), make_column("ObjectHandle", &ShaderMeta::ObjectHandle), make_column("Path", &ShaderMeta::Path));
}

static auto MakeStorage() {
  const auto LibPath = Project::GetLibraryPath();
  const auto Path = ::Path::Combine(LibPath, ASSET_DATABASE_NAME);
  return make_storage(Path.Data(), MakeAssetIndexTable(), MakeMeshMetaTable(), MakeShaderMetaTable());
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
    const auto Metas = GetAll<MetaType>(where(c(&MetaType::ObjectHandle) == ObjectHandle), limit(1));
    if (Metas.empty()) {
      return {};
    }
    return Metas[0];
  }

  template <typename MetaType> Optional<MetaType> QueryMeta(const StringView Path) {
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
    if (!mLoadingAssets.Contains(ObjectHandle)) {
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

AssetsManager::AssetsManager() = default;
AssetsManager::~AssetsManager() = default;

static void WaitAllHandles(Array<AssetLoadTaskHandle> Handles) {
  for (auto& Handle : Handles) {
    Handle.WaitSync();
  }
}

void AssetsManager::StartUp() {
  Shader::ReadCache();
  GetRef().mImpl = MakeUnique<Impl>();
  // clang-format off
  WaitAllHandles({
    ImportAsync("Assets/Mesh/Cube.fbx"),
    ImportAsync("Assets/Shader/Skybox.slang")
  });
  // clang-format on
}

void AssetsManager::ShutDown() {
  Shader::WriteCache();
  // 卸载所有的资产
  auto& Self = GetRef();
  {
    AutoLock Lock(Self.mImpl->mLoadedAssetsMutex);
    for (const auto LoadedAsset : Self.mImpl->mLoadedAssets) {
      if (auto MyAsset = static_cast<Asset*>(ObjectTable::GetObject(LoadedAsset))) {
        if (MyAsset->IsLoaded()) {
          // TODO: AssetManager Unload接口
          MyAsset->BeforeUnload();
          MyAsset->Unload();
          MyAsset->AfterUnload();
        }
        ObjectTable::UnregisterObject(MyAsset, true);
      }
    }
    Self.mImpl->mLoadedAssets.Clear();
  }
  // 等待mLoadingAssets为空
  while (true) {
    AutoLock Lock(Self.mImpl->mLoadingAssetsMutex);
    if (Self.mImpl->mLoadingAssets.Empty()) {
      break;
    }
  }
  // 再次卸载
  {
    AutoLock Lock(Self.mImpl->mLoadedAssetsMutex);
    for (const auto LoadedAsset : Self.mImpl->mLoadedAssets) {
      if (auto MyAsset = static_cast<Asset*>(ObjectTable::GetObject(LoadedAsset))) {
        if (MyAsset->IsLoaded()) {
          // TODO: AssetManager Unload接口
          MyAsset->BeforeUnload();
          MyAsset->Unload();
          MyAsset->AfterUnload();
        }
        ObjectTable::UnregisterObject(MyAsset, true);
      }
    }
    Self.mImpl->mLoadedAssets.Clear();
  }
}

static EAssetType GetAssetTypeByExtension(StringView Path) {
  const StringView Ext = Path::GetExtension(Path);
  if (Ext == "fbx" || Ext == "obj") {
    return EAssetType::Mesh;
  }
  if (Ext == "slang") {
    return EAssetType::Shader;
  }
  return EAssetType::Count;
}

AssetLoadTaskHandle AssetsManager::ImportAsyncM(StringView Path) {
  const EAssetType Type = GetAssetTypeByExtension(Path);
  if (Type == EAssetType::Count) {
    gLogger.Error(Logcat::Asset, "未知的资产类型 '{}'.", Path);
    return {};
  }
  if (auto OpIndex = mImpl->QueryMeta<AssetIndex>(Path)) {
    gLogger.Warn(Logcat::Asset, "资产 '{}' 重复导入, 将跳过此次导入, 但仍会进行加载.", Path);
  } else {
    AssetIndex NewIndex{};
    NewIndex.ObjectHandle = ObjectTable::AssignHandle(true);
    NewIndex.Path = Path.ToString();
    NewIndex.Type = Type;
    mImpl->Insert(NewIndex);

    switch (NewIndex.Type) {
    case EAssetType::Shader: {
      ShaderMeta NewShaderMeta{};
      NewShaderMeta.ObjectHandle = NewIndex.ObjectHandle;
      NewShaderMeta.Path = NewIndex.Path;
      mImpl->Insert(NewShaderMeta);
    } break;
    case EAssetType::Mesh: {
      MeshMeta NewMeshMeta{};
      NewMeshMeta.ObjectHandle = NewIndex.ObjectHandle;
      NewMeshMeta.Path = NewIndex.Path;
      mImpl->Insert(NewMeshMeta);
    } break;
    case EAssetType::Material:
      break;
    case EAssetType::Scene:
      break;
    case EAssetType::Animation:
      break;
    case EAssetType::Audio:
      break;
    case EAssetType::Font:
      break;
    case EAssetType::Prefab:
      break;
    case EAssetType::Count:
      break;
    }
  }
  return LoadAsyncM(Path);
}

Optional<MeshMeta> AssetsManager::QueryMeshMeta(const StringView Path) { return mImpl->QueryMeta<MeshMeta>(Path); }

Optional<MeshMeta> AssetsManager::QueryMeshMeta(const Int32 ObjectHandle) { return mImpl->QueryMeta<MeshMeta>(ObjectHandle); }

bool AssetsManager::IsAssetLoading(const Int32 Handle) { return GetRef().mImpl->IsLoading(Handle); }

bool AssetsManager::IsAssetLoaded(const Int32 Handle) { return GetRef().mImpl->IsLoaded(Handle); }

Array<AssetIndex> AssetsManager::internalGetAllAssetIndex() {
  Array<AssetIndex> Result;
  for (const auto& Index : mImpl->GetAll<AssetIndex>()) {
    Result.Add(Index);
  }
  return Result;
}

struct AssetLoadTask : TaskNode {
  explicit AssetLoadTask(Asset* InAsset) : Asset(InAsset) {}
  [[nodiscard]] virtual ENamedThread GetDesiredThread() const override { return ENamedThread::IO; }

  virtual ETaskNodeResult Run() override {
    if (Asset) {
      Asset->Load();
    }
    return ETaskNodeResult::Success;
  }

  Asset* Asset;
};

struct AssetLoadCompletedTask final : TaskNode {
  AssetLoadCompletedTask(AssetsManager::Impl* InImpl, Int32 InObjectHandle) : Impl(InImpl), ObjectHandle(InObjectHandle) {}

  [[nodiscard]] virtual ENamedThread GetDesiredThread() const override { return ENamedThread::IO; }

  virtual ETaskNodeResult Run() override {
    if (Impl && ObjectHandle != 0) {
      Impl->MakeObjectLoaded(ObjectHandle);
    }
    return ETaskNodeResult::Success;
  }

  AssetsManager::Impl* Impl;
  Int32 ObjectHandle;
};

template <class T, class TMeta> static AssetLoadTaskHandle LoadAssetAsync(const TMeta& Meta, AssetsManager::Impl* Impl) {
  CPU_PROFILING_SCOPE;
  if (const Int32 ObjHandle = Meta.ObjectHandle; AssetsManager::IsAssetLoaded(ObjHandle)) {
    AssetLoadTaskHandle TaskHandle{};
    TaskHandle.internalSetTargetObject(static_cast<Asset*>(ObjectTable::GetObject(ObjHandle)));
    return TaskHandle;
  }
  T* MyShader = CreateObject<T>(Meta.Path);
  ObjectTable::ModifyObjectHandle(MyShader, Meta.ObjectHandle);
  // 首先处理Meta
  MyShader->ApplyMeta(Meta);
  // 然后开始加载
  Impl->MakeObjectLoading(Meta.ObjectHandle);
  const TaskHandle Handle = TaskGraph::CreateTask<AssetLoadTask>(Format("Task: Load shader '{}'", Meta.Path), {}, MyShader);
  TaskGraph::CreateTask<AssetLoadCompletedTask>("Task: AssetLoadCompleted", {Handle}, Impl, Meta.ObjectHandle);
  return AssetLoadTaskHandle(Handle, MyShader);
}

static AssetLoadTaskHandle LoadAssetByIndex(const AssetIndex& Index, AssetsManager::Impl* Impl) {
  switch (Index.Type) {
  case EAssetType::Mesh: {
    auto OpMeshMeta = Impl->QueryMeta<MeshMeta>(Index.ObjectHandle);
    if (!OpMeshMeta) {
      return {};
    }
    return LoadAssetAsync<Mesh>(*OpMeshMeta, Impl);
  }
  case EAssetType::Shader: {
    auto OpShaderMeta = Impl->QueryMeta<ShaderMeta>(Index.ObjectHandle);
    if (!OpShaderMeta) {
      return {};
    }
    return LoadAssetAsync<Shader>(*OpShaderMeta, Impl);
  }
  default:
    return {};
  }
}

AssetLoadTaskHandle AssetsManager::LoadAsyncM(const StringView Path) {
  // 首先从AssetIndex里查询
  CPU_PROFILING_SCOPE;
  auto OpAssetIndex = mImpl->QueryMeta<AssetIndex>(Path);
  if (!OpAssetIndex) {
    gLogger.Error(Logcat::Asset, "LoadAsync: 找不到对象'{}', 是否忘记导入?", Path);
    return {};
  }
  return LoadAssetByIndex(*OpAssetIndex, mImpl.Get());
}

AssetLoadTaskHandle AssetsManager::LoadAsyncM(const Int32 ObjectHandle) {
  // 首先从AssetIndex里查询
  CPU_PROFILING_SCOPE;
  auto OpAssetIndex = mImpl->QueryMeta<AssetIndex>(ObjectHandle);
  if (!OpAssetIndex) {
    gLogger.Error(Logcat::Asset, "LoadAsync: 对象'{}'不存在, 是否忘记导入?", ObjectHandle);
    return {};
  }
  return LoadAssetByIndex(*OpAssetIndex, mImpl.Get());
}
