#pragma once
#include "Core/Macros.h"
#include "Core/Reflection/MetaMark.h"

#include "IConfig.generated.h"

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

protected:
  bool mSaveDirty = false;
#endif
};
