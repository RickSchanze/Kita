#pragma once
#include "Core/Memory/UniquePtr.h"
#include "Core/Singleton/Singleton.h"

class InputTicker;
class RHISurfaceWindow;
struct EngineData {
  /// 当前帧号
  UInt32 FrameIndex;
  /// DeltaTime in seconds
  double DeltaSeconds;
};

class EngineLoop : public Singleton<EngineLoop> {
public:
  static void StartUpSystems(const char** ArgV, const int ArgC) { GetRef().StartUpSystemsM(ArgV, ArgC); }
  /// 所有系统的清理函数
  /// @param Tips 如果为True, 则会打印一条“按任意键结束” 然后等待用户输入 输入了再关终端
  static void ShutDownSystems(bool Tips = false);
  static void Run() { GetRef().RunM(); }

private:
  void StartUpSystemsM(const char** ArgV, int ArgC);
  void ShutDownSystemsM();
  void RunM();

  EngineData mEngineData{};
  /// 引擎主窗口
  RHISurfaceWindow* mSurfaceWindow = nullptr;
  InputTicker* mInputTicker = nullptr;
};
