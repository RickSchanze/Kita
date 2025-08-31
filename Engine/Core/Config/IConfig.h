#pragma once
#include "Core/Macros.h"
#include "Core/Reflection/MetaMark.h"

#include "IConfig.generated.h"

#define KITA_CONFIG_GETTER_SETTER(PropertyType, Property)                            \
  [[nodiscard]] FORCE_INLINE PropertyType Get##Property() const { return Property; } \
  void Set##Property(const PropertyType& InProperty) {                               \
    this->Property = InProperty;                                                     \
    SetSaveDirty(true);                                                              \
  }

KCLASS(Abstract)
class IConfig {
  GENERATED_BODY(IConfig)
public:
  String GetFilePath();
  String GetCategory();

  virtual ~IConfig() = default;
#if KITA_EDITOR
  FORCE_INLINE void SetSaveDirty(const bool Dirty = true) { mSaveDirty = Dirty; }

  [[nodiscard]] FORCE_INLINE bool IsSaveDirty() const { return mSaveDirty; }

  /// 这个出发的时机是在Archive写入完成但是还未实际写入文件时
  virtual void PostSave() {}

protected:
  bool mSaveDirty = false;
#endif
};
