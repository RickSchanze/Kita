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
  virtual Result<void, EReflectionError> Add(const AnyRef& InValue) = 0;
  virtual SizeType Count() const = 0;
  virtual AnyRef Get(SizeType Index) const = 0;
  virtual void SetInstancePtr(void* Ptr) = 0;
};

template <typename T> class ArrayProxy_Array : public ArrayProxy {
public:
  virtual void SetInstancePtr(void* Ptr) override { mInstancePtr = Ptr; }
  virtual Result<void, EReflectionError> Add(const Any& InValue) override {
    if (mInstancePtr == nullptr) {
      return EReflectionError::NullPointer;
    }
    const Type* ElementType = TypeOf<T>();
  }

private:
  Array<T>* mInstancePtr = nullptr;
};
