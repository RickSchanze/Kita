//
// Created by kita on 25-8-21.
//

#include "ObjectTable.h"

bool ObjectTable::IsObjectAlive(const Int32 Handle) const {
  CPU_PROFILING_SCOPE;
  std::lock_guard Lock(mObjectTable);
  return mObjectTable.Contains(Handle);
}

bool ObjectTable::RegisterObject(Object* Object) {
  CPU_PROFILING_SCOPE;
  if (Object == nullptr)
    return false;
  Int32 ObjectHandle = Object->GetObjectHandle();
  if (IsObjectAlive(ObjectHandle)) {
    gLogger.Error("Object", "重复向ObjectTable添加同一ID的Object, Id = {}", ObjectHandle);
    return false;
  }
  std::lock_guard Lock(mObjectTable);
  mObjectTable.Add(ObjectHandle, Object);
  return true;
}

void ObjectTable::UnregisterObject(const Int32 ObjectHandle, bool Delete) {
  CPU_PROFILING_SCOPE;
  if (IsObjectAlive(ObjectHandle)) {
    std::lock_guard Lock(mObjectTable);
    Object* Object = mObjectTable[ObjectHandle];
    mObjectTable.Remove(ObjectHandle);
    if (Delete) {
      ::Delete(Object);
    }
  }
}

void ObjectTable::UnregisterObject(const Object* Object, bool Delete) {
  CPU_PROFILING_SCOPE;
  std::lock_guard Lock(mObjectTable);
  if (Object != nullptr)
    UnregisterObject(Object->GetObjectHandle(), Delete);
}

Int32 ObjectTable::AssignHandle(const bool IsPersistent) {
  CPU_PROFILING_SCOPE;
  if (IsPersistent) {
    std::lock_guard Lock(mPersistentId);
    return mPersistentId++;
  } else {
    std::lock_guard Lock(mInstancedId);
    return mInstancedId++;
  }
}

Object* ObjectTable::CreateObject(const Type* InType, const StringView NewName) {
  if (InType == nullptr)
    return nullptr;
  CPU_PROFILING_SCOPE;
  Object* NewObject = InType->CreateInstanceT<Object>();
  NewObject->SetObjectName(NewName);
  const Int32 Id = AssignHandle(false);
  NewObject->InternalSetObjectHandle(Id);
  RegisterObject(NewObject);
  return NewObject;
}

void ObjectTable::ModifyObjectHandle(Object* Object, Int32 NewHandle) {
  if (Object == nullptr || Object->GetObjectHandle() == NewHandle) {
    return;
  }
  if (NewHandle == 0) {
    gLogger.Error(Logcat::Object, "更新对象Handle失败, NewHandle=0. 对象: '{}', {:p}.", Object->GetObjectName(), Ptr(Object));
    return;
  }
  UnregisterObject(Object, false);
  Object->InternalSetObjectHandle(NewHandle);
  RegisterObject(Object);
}
