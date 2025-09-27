#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Core/TypeDefs.h"

#include "ObjectPtr.generated.h"

/// 封装ObjectHandle
/// 极致性能的场景下请自己使用Object* 但是此时无法使用反射系统
KSTRUCT()
struct ObjectPtrBase {
  GENERATED_BODY(ObjectPtrBase)
  ObjectPtrBase() = default;
  ~ObjectPtrBase() = default;

  /// 设置此ObjectHandle
  void SetObjectHandle(Int32 NewHandle);
  [[nodiscard]] Int32 GetHandle() const { return mObjectHandle; }

  void GetReferencingObject(Array<Int32>& OutArray) { OutArray.Add(mObjectHandle); }

private:
  KPROPERTY()
  Int32 mObjectHandle = 0;
};

template <typename T> struct ObjectPtr : ObjectPtrBase, TemplateAdapter {
  using ReflectionType = T;
  ObjectPtr() = default;
  ~ObjectPtr() = default;
};

template <typename T> void ExtractReferencingObjectHandle(const ObjectPtr<T>& Ptr, Array<Int32>& Out) {
  if (Ptr.GetHandle() != 0) {
    Out.Add(Ptr.GetHandle());
  }
}

template <typename T> void ExtractReferencingObjectHandle(const Array<ObjectPtr<T>>& PtrArray, Array<Int32>& Out) {
  for (auto& Ptr : PtrArray) {
    if (Ptr.GetHandle() != 0) {
      Out.Add(Ptr.GetHandle());
    }
  }
}

template <typename K, typename T> void ExtractReferencingObjectHandle(const Map<K, ObjectPtr<T>>& PtrMap, Array<Int32>& Out) {
  for (auto& Ptr : PtrMap) {
    if (Ptr.second.GetHandle() != 0) {
      Out.Add(Ptr.second.GetHandle());
    }
  }
}
