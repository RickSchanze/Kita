#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Object.h"

#include "PersistentObject.generated.h"

KCLASS()
class PersistentObject : public Object {
  GENERATED_BODY(PersistentObject)
public:
  PersistentObject() = default;
  virtual ~PersistentObject() override = default;

  [[nodiscard]] bool IsLoaded() const { return mLoaded; }

  virtual void Load() { mLoaded = true; }
  virtual void Unload() { mLoaded = false; }

  virtual void OnLoadComplete() {}

protected:
  void SetLoaded(const bool Loaded) {
    mLoaded = Loaded;
    if (Loaded)
      OnLoadComplete();
  }

  bool mLoaded = false;
};
