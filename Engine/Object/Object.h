#pragma once

#include "Core/Macros.h"
#include "Core/String/String.h"

#include "Object.generated.h"

KCLASS()
class Object {
  GENERATED_BODY(Object)
public:
  Object() = default;
  virtual ~Object() = default;

  void SetObjectName(const StringView Name) { mName = String(Name); }
  [[nodiscard]] StringView GetObjectName() const { return mName; }

  /// 供内部使用的设置对象Handle的方法
  void InternalSetObjectHandle(const Int32 NewHandle) { mHandle = NewHandle; }
  [[nodiscard]] Int32 GetObjectHandle() const { return mHandle; }

protected:
  /// 对象的名字
  KPROPERTY()
  String mName;

  /// 对象句柄(ID) 为0代表无效 >0为持久化对象 <0为实例对象
  KPROPERTY()
  Int32 mHandle = 0;
};

namespace Traits {
template <typename T>
concept HasReferencingObjectFunc = requires(T Value, Array<Int32>& Array) {
  { Value.GetReferencingObject(Array) } -> std::convertible_to<void>;
};
} // namespace Traits
