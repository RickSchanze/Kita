#pragma once
#include "Macros.h"

#define ASSERT_MSG(Condition, ...)           \
  if (!(Condition)) {                        \
    gLogger.Critical("Assert", __VA_ARGS__); \
  }

#define ASSERT(Condition)                                        \
  if (!(Condition)) {                                            \
    gLogger.Critical("Assert", "Assertion failed: " #Condition); \
  }

#ifdef KITA_DEBUG
#define DEBUG_ASSERT_MSG(Condition, ...)  \
  if (!(Condition)) {                     \
    gLogger.Error("Assert", __VA_ARGS__); \
  }

#define DEBUG_ASSERT(Condition)                               \
  if (!(Condition)) {                                         \
    gLogger.Error("Assert", "Assertion failed: " #Condition); \
  }
#else
#define DEBUG_ASSERT_MSG(Condition, ...)
#define DEBUG_ASSERT(Condition)
#endif