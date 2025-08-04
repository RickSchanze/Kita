#pragma once
#include "Any.h"
#include "Core/Container/Array.h"
#include "Core/Container/Result.h"
#include "Core/TypeDefs.h"
#include "TypeRegistry.h"

struct Type;
class ArrayProxy {
public:
  virtual ~ArrayProxy() = default;
  virtual Result<void, EReflectionError> Add(AnyRef& InValue) = 0;
  [[nodiscard]] virtual SizeType Count() const = 0;
  [[nodiscard]] virtual Result<AnyRef, EReflectionError> Get(SizeType Index) const = 0;
  virtual void SetInstancePtr(void* Ptr) = 0;
  virtual void ClearInstancePtr() = 0;
};

template <typename T> class ArrayProxy_Array : public ArrayProxy {
public:
  virtual void SetInstancePtr(void* Ptr) override { mInstancePtr = Ptr; }
  virtual Result<void, EReflectionError> Add(AnyRef& InValue) override {
    if (!mInstancePtr) {
      return EReflectionError::NullPointer;
    }
    auto Result = InValue.Cast<T>();
    if (!Result) {
      return Result.Error();
    }
    mInstancePtr->Add(*Result);
    return {};
  }

  [[nodiscard]] virtual SizeType Count() const override {
    if (!mInstancePtr) {
      return -1;
    }
    return mInstancePtr->Count();
  }

  [[nodiscard]] virtual Result<AnyRef, EReflectionError> Get(SizeType Index) const override {
    if (!mInstancePtr) {
      return EReflectionError::NullPointer;
    }
    if (Index >= mInstancePtr->Count()) {
      return EReflectionError::OutOfRange;
    }
    AnyRef Ref = (*mInstancePtr)[Index];
    return Ref;
  }

  virtual void ClearInstancePtr() override { mInstancePtr = nullptr; }

private:
  Array<T>* mInstancePtr = nullptr;
};

template <typename T> UniquePtr<ArrayProxy> CreateArrayProxy() {
  if constexpr (Traits::IsArray<T>) {
    return MakeUnique<ArrayProxy_Array<T>>();
  } else {
    return {};
  }
}
