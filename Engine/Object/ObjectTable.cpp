//
// Created by kita on 25-8-21.
//

#include "ObjectTable.h"

void ObjectTable::StartUp() {

}

void ObjectTable::ShutDown() {

}

bool ObjectTable::IsObjectAlive(const Int32 Handle) {
  CPU_PROFILING_SCOPE;
  AutoLock Lock(mTableMutex);
  return mObjectTable.Contains(Handle);
}

bool ObjectTable::RegisterObject(Object* Object) {
  CPU_PROFILING_SCOPE;
  if (Object == nullptr)
    return false;
  Int32 ObjectHandle = Object->GetHandle();
  if (IsObjectAlive(ObjectHandle)) {
    gLogger.Error("Object", "重复向ObjectTable添加同一ID的Object, Id = {}", ObjectHandle);
    return false;
  }
  AutoLock Lock(mTableMutex);
  mObjectTable.Add(ObjectHandle, Object);
  return true;
}

void ObjectTable::UnregisterObjectM(const Int32 ObjectHandle, bool Delete) {
  CPU_PROFILING_SCOPE;
  if (IsObjectAlive(ObjectHandle)) {
    AutoLock Lock(mTableMutex);
    Object* Object = mObjectTable[ObjectHandle];
    mObjectTable.Remove(ObjectHandle);
    if (Delete) {
      ::Delete(Object);
    }
  }
}

void ObjectTable::UnregisterObjectM(const Object* Object, bool Delete) {
  CPU_PROFILING_SCOPE;
  if (Object != nullptr) {
    UnregisterObjectM(Object->GetHandle(), Delete);
  }
}

Int32 ObjectTable::AssignHandleM(const bool IsPersistent) {
  CPU_PROFILING_SCOPE;
  if (IsPersistent) {
    AutoLock Lock(mPersistentIdMutex);
    return mPersistentId++;
  } else {
    AutoLock Lock(mInstancedIdMutex);
    return mInstancedId++;
  }
}

Object* ObjectTable::CreateObject(const Type* InType, const StringView NewName) {
  if (InType == nullptr)
    return nullptr;
  CPU_PROFILING_SCOPE;
  Object* NewObject = InType->CreateInstanceT<Object>();
  NewObject->SetObjectName(NewName);
  const Int32 Id = AssignHandleM(false);
  NewObject->InternalSetHandle(Id);
  RegisterObject(NewObject);
  return NewObject;
}

void ObjectTable::ModifyObjectHandleM(Object* Object, Int32 NewHandle) {
  CPU_PROFILING_SCOPE;
  if (Object == nullptr || Object->GetHandle() == NewHandle) {
    return;
  }
  if (NewHandle == 0) {
    gLogger.Error(Logcat::Object, "更新对象Handle失败, NewHandle=0. 对象: '{}', {:p}.", Object->GetObjectName(), Ptr(Object));
    return;
  }
  UnregisterObjectM(Object, false);
  Object->InternalSetHandle(NewHandle);
  RegisterObject(Object);
}

Object* ObjectTable::GetObjectM(const Int32 Handle) {
  AutoLock Lock(mTableMutex);
  if (const auto Iter = mObjectTable.Find(Handle); Iter != mObjectTable.End()) {
    return Iter->second;
  }
  return nullptr;
}
