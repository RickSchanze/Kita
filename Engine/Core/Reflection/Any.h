#pragma once
#include "Core/Container/Result.h"
#include "Core/Memory/SharedPtr.h"
#include "Core/Memory/UniquePtr.h"
#include "TypeRegistry.h"

struct Type;
enum class EReflectionError {
  TypeMismatch,
  NullPointer,
  OutOfRange,
};

class Any {
public:
  class DataHolder {
  public:
    virtual ~DataHolder() = default;
    const Type* GetType() const { return mType; }

    explicit DataHolder(const Type* InType) : mType(InType) {}

    virtual void* GetDataPtr() = 0;

  protected:
    const Type* mType = nullptr;
  };

  template <typename T> class DataHolder_T : public DataHolder {
    static_assert(std::is_copy_constructible_v<T>, "T must be copy constructible");

    T mData;

  public:
    explicit DataHolder_T(const T& InData) : DataHolder(TypeOf<T>()), mData(InData) {}
    // 复制构造
    DataHolder_T(const DataHolder_T& Other) : DataHolder(Other.mType), mData(Other.mData) {}

    virtual void* GetDataPtr() override { return &mData; }
  };

private:
  SharedPtr<DataHolder> mDataHolder;

public:
  const Type* GetType() const { return mDataHolder->GetType(); }

  template <typename T> explicit Any(const T& InData) : mDataHolder(MakeShared<DataHolder_T<Traits::Pure<T>>>(InData)) {}

  void* GetDataPtr() { return mDataHolder->GetDataPtr(); }
};

class AnyRef {
public:
  class DataHolder {
  public:
    virtual ~DataHolder() = default;
    const Type* GetType() const { return mType; }

    explicit DataHolder(const Type* InType) : mType(InType) {}

    virtual void* GetDataPtr() = 0;

  protected:
    const Type* mType = nullptr;
  };

  template <typename T> class DataHolder_T : public DataHolder {
    static_assert(std::is_copy_constructible_v<T>, "T must be copy constructible");

    T* mData;

  public:
    explicit DataHolder_T(T* InData) : DataHolder(TypeOf<T>()), mData(InData) {}
    // 复制构造
    DataHolder_T(const DataHolder_T& Other) : DataHolder(Other.mType), mData(Other.mData) {}

    virtual void* GetDataPtr() override { return mData.Get(); }
  };

private:
  SharedPtr<DataHolder> mDataHolder;

public:
  const Type* GetType() const { return mDataHolder->GetType(); }

  template <typename T> explicit AnyRef(const T& InData) : mDataHolder(MakeShared<DataHolder_T<Traits::Pure<T>>>(&InData)) {}

  void* GetDataPtr() { return mDataHolder->GetDataPtr(); }

  template <typename T>
  Result<T*, EReflectionError> Cast() {
    if (mDataHolder->GetType() != TypeOf<T>()) {
      return EReflectionError::TypeMismatch;
    }
    if (mDataHolder->GetDataPtr() == nullptr) {
      return EReflectionError::NullPointer;
    }
    return static_cast<T*>(mDataHolder->GetDataPtr());
  }
};
