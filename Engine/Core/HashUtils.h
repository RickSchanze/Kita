#pragma once
#include "TypeDefs.h"

class HashUtils {
public:
  static constexpr UInt64 FNV_OFFSET_BASIS = 14695981039346656037ULL;
  static constexpr UInt64 FNV_PRIME = 1099511628211ULL;

  static uint64_t HashCombine(UInt64 Seed, const UInt64 Value) {
    Seed ^= Value;
    Seed *= FNV_PRIME;
    return Seed;
  }
};
