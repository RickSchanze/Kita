#pragma once
#include "Core/Container/Array.h"
struct Type;

class TypeRegistry : NonCopyable, NonMovable {
public:
  TypeRegistry();
  ~TypeRegistry();

  void RegisterType(const Type* Type);
  const Type* GetType(const StringView& TypeName) const;
  const Type* GetType(SizeType& HashCode) const;

private:
  Array<const Type*> mRegisteredTypes;
};

inline TypeRegistry gTypeRegistry;

inline TypeRegistry& GetTypeRegistry() { return gTypeRegistry; }
