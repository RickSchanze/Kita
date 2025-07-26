//
// Created by kita on 25-7-26.
//

#include "Type.h"
#include "TypeRegistry.h"

void Pri::RegisterTypeToRegistry(const Type* Type) { GetTypeRegistry().RegisterType(Type); }