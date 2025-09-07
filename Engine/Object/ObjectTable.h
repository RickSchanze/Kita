#pragma once
#include "Core/Container/Map.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/Singleton/Singleton.h"
#include "Object.h"

class ObjectTable : public Singleton<ObjectTable> {
public:
  static void StartUp();
  static void ShutDown();

  [[nodiscard]] bool IsObjectAlive(Int32 Handle);

  /// 注册一个Object 如果此Object对应的Handle已存在则Log错误并返回false
  bool RegisterObject(Object* Object);

  /// 从ObjectTable中删除一个Object, 同时释放内存 TODO: 延迟内存释放
  /// @param ObjectHandle 要删除的Object的Handle
  /// @param Delete 是否要释放内存
  static void UnregisterObject(const Int32 ObjectHandle, const bool Delete) { GetRef().UnregisterObjectM(ObjectHandle, Delete); }
  void UnregisterObjectM(Int32 ObjectHandle, bool Delete);

  /// 从ObjectTable中删除一个Object
  /// @param Object 要删除的Object
  /// @param Delete 是否要释放内存
  static void UnregisterObject(const Object* Object, const bool Delete) { GetRef().UnregisterObjectM(Object, Delete); }
  void UnregisterObjectM(const Object* Object, bool Delete);

  static Int32 AssignHandle(const bool IsPersistent) { return GetRef().AssignHandleM(IsPersistent); }
  Int32 AssignHandleM(bool IsPersistent);

  /// 以Type创建一个Object 最好使用模版化版本性能能稍好一些
  /// @param InType 输入类型
  /// @param NewName 新Object的名称
  /// @return
  Object* CreateObject(const Type* InType, StringView NewName = "NewObject");

  /// 创建特定类型的Object
  /// @tparam T
  /// @return
  template <typename T>
    requires Traits::IsBaseOf<Object, T>
  T* CreateObject(StringView NewName = "NewObject") {
    CPU_PROFILING_SCOPE;
    Int32 NewId = AssignHandleM(false);
    T* NewObject = new T();
    NewObject->InternalSetHandle(NewId);
    NewObject->SetObjectName(NewName);
    RegisterObject(NewObject);
    return NewObject;
  }

  /// 更改一个Object的Handle
  /// @warning 请知道你在调用此函数时在做什么, 调用此函数会导致所有对此对象的引用都失效, 目前只在资产系统中使用
  /// @param Object
  /// @param NewHandle
  void ModifyObjectHandleM(Object* Object, Int32 NewHandle);
  static void ModifyObjectHandle(Object* Object, const Int32 NewHandle) { ObjectTable::GetRef().ModifyObjectHandleM(Object, NewHandle); }

  static Object* GetObject(const Int32 Handle) { return ObjectTable::GetRef().GetObjectM(Handle); }
  Object* GetObjectM(Int32 Handle);

private:
  // TODO: 更换为ConcurrentMap
  DECL_TRACKABLE_MUREX(std::mutex, mTableMutex);
  Map<Int32, Object*> mObjectTable;

  DECL_TRACKABLE_MUREX(std::mutex, mPersistentIdMutex);
  Int32 mPersistentId = 1;

  DECL_TRACKABLE_MUREX(std::mutex, mInstancedIdMutex);
  Int32 mInstancedId = -1;
};

template <typename T>
  requires Traits::IsBaseOf<Object, T>
T* CreateObject(StringView NewName) {
  return ObjectTable::GetRef().CreateObject<T>(NewName);
}

inline Object* CreateObject(const Type* ObjectType, const StringView NewName) { return ObjectTable::GetRef().CreateObject(ObjectType, NewName); }
