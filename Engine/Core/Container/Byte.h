#pragma once
#include "Core/Assert.h"
#include "Core/Logging/Logger.hpp"

struct Byte {
  unsigned char Value;

  // 构造函数
  Byte(const unsigned char Val = 0) : Value(Val) {}

  // 测试某一位是否为1
  // Pos: 位位置 (0-7, 0表示最低位)
  [[nodiscard]] bool Test(const int Pos) const {
    ASSERT_MSG(Pos >= 0 && Pos <= 7, "Position must be between 0 and 7");
    return (Value & (1 << Pos)) != 0;
  }

  // 将某一位设为1
  // Pos: 位位置 (0-7, 0表示最低位)
  void Set(const int Pos) {
    ASSERT_MSG(Pos >= 0 && Pos <= 7, "Position must be between 0 and 7");
    Value |= (1 << Pos);
  }

  // 将某一位设为0
  // Pos: 位位置 (0-7, 0表示最低位)
  void Clear(const int Pos) {
    ASSERT_MSG(Pos >= 0 && Pos <= 7, "Position must be between 0 and 7");
    Value &= ~(1 << Pos);
  }
};
