#pragma once
#include "Core/Reflection/MetaMark.h"

#include "ITick.generated.h"

KCLASS(Abstract)
class ITick {
  GENERATED_BODY(ITick)
public:
  /// 默认构造函数将自己加入TickManager
  ITick();

  /// 析构函数将自己从TickManager中移除
  virtual ~ITick();

  virtual void Tick(double DeltaSeconds) {}

#ifdef KITA_DEBUG
  void SetTickDebugName(StringView Name);
  StringView GetTickDebugName() const { return mDebugTickName; }
#endif

private:
#if KITA_DEBUG
  String mDebugTickName;
#endif
};
