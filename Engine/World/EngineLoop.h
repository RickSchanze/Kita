#pragma once
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
  static void StartUpSystems() { GetRef().StartUpSystemsM(); }
  static void ShutDownSystems() { GetRef().ShutDownSystemsM(); }
  static void Run() { GetRef().RunM(); }

private:
  void StartUpSystemsM();
  void ShutDownSystemsM();
  void RunM();

  EngineData mEngineData{};
  /// 引擎主窗口
  RHISurfaceWindow* mSurfaceWindow = nullptr;
  InputTicker* mInputTicker = nullptr;
};
