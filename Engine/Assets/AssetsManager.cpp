//
// Created by kita on 25-9-3.
//

#include "AssetsManager.h"

#include "Core/Performance/ProfilerMark.h"
#include "Mesh.h"
#include "Project/Project.h"
#include "sqlite_orm/sqlite_orm.h"

using namespace sqlite_orm;

AssetsManager::AssetsManager() = default;
AssetsManager::~AssetsManager() = default;

void AssetsManager::StartUp() {}

void AssetsManager::ShutDown() {}

constexpr auto ASSET_CENTER_DATABASE_NAME = "AssetCenter.db";

#pragma region AssetType的SqliteOrm适配
//  also we need transform functions to make string from enum..
inline std::string AssetTypeToString(const EAssetType AssetType) {
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

/**
 *  and enum from string. This function has nullable result cause
 *  string can be neither `male` nor `female`. Of course we
 *  won't allow this to happed but as developers we must have
 *  a scenario for this case.
 *  These functions are just helpers. They will be called from several places
 *  that's why I placed it separatedly. You can use any transformation type/form
 *  (for example BETTER_ENUM https://github.com/aantron/better-enums)
 */
inline std::unique_ptr<EAssetType> AssetTypeFromString(const std::string& s) {
  if (s == "Shader") {
    return std::make_unique<EAssetType>(EAssetType::Shader);
  }
  if (s == "Material") {
    return std::make_unique<EAssetType>(EAssetType::Material);
  }
  return nullptr;
}

/**
 *  This is where magic happens. To tell sqlite_orm how to act
 *  with Gender enum we have to create a few service classes
 *  specializations (traits) in sqlite_orm namespace.
 */
namespace sqlite_orm {

/**
 *  First of all is a type_printer template class.
 *  It is responsible for sqlite type string representation.
 *  We want Gender to be `TEXT` so let's just derive from
 *  text_printer. Also there are other printers: real_printer and
 *  integer_printer. We must use them if we want to map our type to `REAL` (double/float)
 *  or `INTEGER` (int/long/short etc) respectively.
 */
template <> struct type_printer<EAssetType> : text_printer {};

/**
 *  This is a binder class. It is used to bind c++ values to sqlite queries.
 *  Here we have to create gender string representation and bind it as string.
 *  Any statement_binder specialization must have `int bind(sqlite3_stmt*, int, const T&)` function
 *  which returns bind result. Also you can call any of `sqlite3_bind_*` functions directly.
 *  More here https://www.sqlite.org/c3ref/bind_blob.html
 */
template <> struct statement_binder<EAssetType> {

  static int bind(sqlite3_stmt* stmt, const int index, const EAssetType& value) {
    return statement_binder<std::string>().bind(stmt, index, AssetTypeToString(value));
    //  or return sqlite3_bind_text(stmt, index++, AssetTypeToString(value).c_str(), -1, SQLITE_TRANSIENT);
  }
};

/**
 *  field_printer is used in `dump` and `where` functions. Here we have to create
 *  a string from mapped object.
 */
template <> struct field_printer<EAssetType> {
  std::string operator()(const EAssetType& t) const { return AssetTypeToString(t); }
};

/**
 *  This is a reverse operation: here we have to specify a way to transform string received from
 *  database to our Gender object. Here we call `AssetTypeFromString` and throw `std::runtime_error` if it returns
 *  nullptr. Every `row_extractor` specialization must have `extract(const char*)` and `extract(sqlite3_stmt *stmt,
 * int columnIndex)` functions which return a mapped type value.
 */
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
  return make_storage(LibPath.Data(), MakeAssetIndexTable(), MakeAssetIndexTable(), MakeMeshMetaTable());
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
    return *(Metas[0]);
  }

  template <typename MetaType> Optional<MetaType> QueryMeta(StringView Path) {
    std::lock_guard Lock(mDatabaseMutex);
    const auto Metas = GetAll<MetaType>(where(c(&MetaType::Path) == Path), limit(1));
    if (Metas.empty()) {
      return {};
    }
    return *(Metas[0]);
  }

  decltype(MakeStorage()) mStorage;
  DECL_TRACKABLE_MUREX(std::mutex, mDatabaseMutex);
};

Optional<MeshMeta> AssetsManager::QueryMeshMeta(const StringView Path) { return mImpl->QueryMeta<MeshMeta>(Path); }

Optional<MeshMeta> AssetsManager::QueryMeshMeta(const Int32 ObjectHandle) { return mImpl->QueryMeta<MeshMeta>(ObjectHandle); }
