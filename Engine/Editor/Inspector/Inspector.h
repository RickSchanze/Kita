#pragma once
#include "Core/Reflection/MetaMark.h"

#include "Inspector.generated.h"

class Object;
struct Type;

KCLASS(Abstract)
class Inspector {
  GENERATED_BODY(Inspector)
public:
  virtual ~Inspector() = default;

  virtual void DrawGUI() = 0;

  Object* GetInspectedObject() const { return mInspectedObject; }
  void SetInspectedObject(Object* InObject);
  virtual void OnSetInspectedObject(Object* NewObj) {}

protected:
  static void Header(StringView Path, Int32 ObjectHandle);
  static void Property(StringView Name, StringView Value);

  template <typename T> static void Property(StringView Name, const T& Value) { Property(Name, static_cast<StringView>(::ToString(Value))); }

  Object* mInspectedObject = nullptr;
};
