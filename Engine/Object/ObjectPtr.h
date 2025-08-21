#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Core/TypeDefs.h"

#include "ObjectPtr.generated.h"

/// 对于
/// KCLASS() class DObject {
///   KPROPERTY()
///   ObjectPtr<PObject> P;
/// };
/// DObject D;
/// 而言 D.P的ObjectHandle即P自身 D.P的ObjectOwner即D
KSTRUCT()
struct ObjectPtrBase {
  GENERATED_BODY(ObjectPtrBase)
  ObjectPtrBase() = default;
  ~ObjectPtrBase() = default;

  /// 设置此ObjectHandle
  void SetObjectHandle(Int32 NewHandle);

  /// 设置此Object的Owner, 此函数仅应由内部调用
  void InternalSetOwner(Int32 NewOwner);

private:
  KPROPERTY()
  Int32 ObjectHandle = 0;

  Int32 ObjectOwner = 0;
};

template <typename T> struct ObjectPtr : ObjectPtrBase {
  ObjectPtr() = default;
  ~ObjectPtr() = default;
};
