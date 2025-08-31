//
// Created by kita on 25-8-21.
//

#include "ObjectTable.h"

bool ObjectTable::RegisterObject(Object* Object) {
  if (Object == nullptr)
    return false;
  Int32 ObjectHandle = Object->GetObjectHandle();
  if (IsObjectAlive(ObjectHandle)) {
    gLogger.Error("Object", "重复向ObjectTable添加同一ID的Object, Id = {}", ObjectHandle);
    return false;
  }
  mObjectTable.Add(ObjectHandle, Object);
  return true;
}

void ObjectTable::UnregisterObject(const Int32 ObjectHandle) {
  if (IsObjectAlive(ObjectHandle)) {
    Object* Object = mObjectTable[ObjectHandle];
    Delete(Object);
    mObjectTable.Remove(ObjectHandle);
  }
}

void ObjectTable::UnregisterObject(const Object* Object) {
  if (Object != nullptr)
    UnregisterObject(Object->GetObjectHandle());
}