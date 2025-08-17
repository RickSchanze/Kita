#pragma once

enum class ENamedThread {
  Game,   // 游戏线程
  Render, // 渲染线程
  IO,     // IO线程池,
};
