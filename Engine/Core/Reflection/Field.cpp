//
// Created by kita on 25-7-27.
//

#include "Field.h"
#include "Core/Assert.h"
#include "Core/Logging/Logger.hpp"
#include "Core/Trace.h"

void Field::SetAttribute(StringView Key, const StringView Value) {
  DEBUG_ASSERT_MSG(!mAttributes.Contains(Key), Format("TypeAttribute {} already registered", Key));
  mAttributes[Key] = Value;
}

void* Field::GetInstanceAddress(void* Instance) const {
  if (IsEnumField())
    return nullptr;
  if (Instance == nullptr)
    return nullptr;
  return static_cast<UInt8*>(Instance) + GetOffset();
}

bool Field::IsEnumField() const { return GetOffset() == -1; }