//
// Created by kita on 25-8-3.
//

#include "Any.h"

#include "TypeRegistry.h"

const Type* Pri::GetTypeByHashCode(SizeType HashCode) {
  return GetTypeRegistry().GetType(HashCode);
}

