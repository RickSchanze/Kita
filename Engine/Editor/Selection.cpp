//
// Created by kita on 25-9-13.
//

#include "Selection.h"

#include "Object/Object.h"

void Selection::StartUp() {}

void Selection::ShutDown() {}

void Selection::SetSelected(const Object* Object) {
  if (Object == nullptr) {
    return;
  }
  auto& Self = GetRef();
  Self.mObjects.Clear();
  Self.mObjects.Add(Object->GetHandle());
}

void Selection::SetSelected(const Array<Object*>& Objects) {
  if (Objects.Count() == 0) {
    return;
  }
  auto& Self = GetRef();
  Self.mObjects.Clear();
  for (const auto Object : Objects) {
    if (Object != nullptr) {
      Self.mObjects.Add(Object->GetHandle());
    }
  }
}

void Selection::SetSelected(Int32 ObjHandle) {
  if (ObjHandle == 0) {
    return;
  }
  auto& Self = GetRef();
  Self.mObjects.Clear();
  Self.mObjects.Add(ObjHandle);
}

void Selection::SetSelected(const Array<Int32>& ObjHandles) {
  if (ObjHandles.Count() == 0) {
    return;
  }
  auto& Self = GetRef();
  Self.mObjects.Clear();
  for (auto ObjHandle : ObjHandles) {
    Self.mObjects.Add(ObjHandle);
  }
}

Int32 Selection::GetFirstSelectedObjectHandle() {
  auto& Self = GetRef();
  if (Self.mObjects.Count() == 0)
    return 0;
  return Self.mObjects[0];
}