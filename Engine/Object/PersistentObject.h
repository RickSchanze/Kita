#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Object.h"

#include "PersistentObject.generated.h"

KCLASS(Abstract)
class PersistentObject : public Object {
  GENERATED_BODY(PersistentObject)
public:
  PersistentObject() = default;
  virtual ~PersistentObject() override = default;

  [[nodiscard]] bool IsLoaded() const { return mLoaded; }
  virtual void BeforeLoad() {}
  virtual void Load() = 0;
  virtual void AfterLoad() {}

  virtual void BeforeUnload() {}
  virtual void Unload() = 0;
  virtual void AfterUnload() {}

  virtual void BeforeSave() {}
  virtual void Save() = 0;
  virtual void AfterSave() {}

protected:
  KPROPERTY()
  String mPath;

  bool mLoaded = false;
};
