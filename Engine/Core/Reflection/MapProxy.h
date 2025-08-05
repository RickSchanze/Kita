#pragma once
#include "Any.h"
#include "Core/Memory/UniquePtr.h"
#include "Core/TypeDefs.h"
#include "Type.h"

class MapProxy {
public:
  virtual ~MapProxy() = default;
  [[nodiscard]] virtual SizeType Count() const = 0;
  [[nodiscard]] virtual const Type* GetKeyType() const = 0;
  [[nodiscard]] virtual const Type* GetValueType() const = 0;
  [[nodiscard]] virtual Result<void, EReflectionError> Add(const AnyRef& Key, const AnyRef& Value) = 0;
  [[nodiscard]] virtual Result<AnyRef, EReflectionError> Get(const AnyRef& Key) = 0;
  virtual void SetInstancePtr(void* Ptr) = 0;
  [[nodiscard]] virtual void* GetInstancePtr() const = 0;
};

template <typename K, typename V> class MapProxy_Map : public MapProxy {
public:
  [[nodiscard]] virtual SizeType Count() const override {
    if (!mInstancePtr) {
      return -1;
    }
    return mInstancePtr->Count();
  }

  virtual void SetInstancePtr(void* Ptr) override { mInstancePtr = Ptr; }
  [[nodiscard]] virtual void* GetInstancePtr() const override { return mInstancePtr; }
  [[nodiscard]] virtual const Type* GetKeyType() const override { return Pri::GetTypeByHashCode(GetTypeStaticHashCode<K>()); }
  [[nodiscard]] virtual const Type* GetValueType() const override { return Pri::GetTypeByHashCode(GetTypeStaticHashCode<V>()); }
  [[nodiscard]] virtual Result<void, EReflectionError> Add(const AnyRef& Key, const AnyRef& Value) override {
    if (Key.GetType() != GetKeyType()) {
      return EReflectionError::TypeMismatch;
    }
    if (Value.GetType() != GetValueType()) {
      return EReflectionError::TypeMismatch;
    }
    if (!mInstancePtr) {
      return EReflectionError::NullPointer;
    }
    auto& MyKey = *Key.Cast<K>().Value();
    auto& MyValue = *Value.Cast<V>().Value();
    mInstancePtr->Add(MyKey, MyValue);
    return {};
  }

private:
  Map<K, V>* mInstancePtr = nullptr;
};

template <typename T> UniquePtr<MapProxy> CreateMapProxy() {
  if constexpr (Traits::IsMap<T>) {
    return MakeUnique<MapProxy_Map<typename Traits::MapTraits<T>::KeyType, typename Traits::MapTraits<T>::ValueType>>();
  } else {
    return {};
  }
}
