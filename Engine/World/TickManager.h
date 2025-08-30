#pragma once
#include "Core/Container/Array.h"
#include "Core/Singleton/Singleton.h"
#include "ITick.h"

/// TickManager按照ITick注册实例顺序进行Tick
class TickManager : public Singleton<TickManager> {
public:
  static void StartUp() {}

  /// Debug模式会输出所有未销毁的Tick的Debug名称
  /// TickManager并不拥有Tick对象 生命周期需要自己管理
  static void ShutDown();

  /// 注册一个Tick对象
  static void RegisterTickInstance(ITick* Instance) { GetRef().RegisterTickInstanceM(Instance); }
  void RegisterTickInstanceM(ITick* Instance);

  /// 卸载一个Tick对象, 注意仅仅从列表中删除, 不会销毁Tick对象
  static void UnRegisterTickInstance(ITick* Instance) { GetRef().UnRegisterTickInstanceM(Instance); }
  void UnRegisterTickInstanceM(ITick* Instance);

  /// 运行所有Tick对象
  static void TickAllObjects(double DeltaSeconds) { GetRef().TickAllObjectsM(DeltaSeconds); }
  void TickAllObjectsM(double DeltaSeconds);

  ITick* GetRenderTickInstance(ITick* Instance) const { return mRenderTickInstance; }
  void SetRenderTickInstance(ITick* Instance) { mRenderTickInstance = Instance; }

  ITick* GetTickInstance(ITick* Instance) const { return mInputTickInstance; }
  void SetTickInstance(ITick* Instance) { mInputTickInstance = Instance; }

private:
  Array<ITick*> mTickObjects;
  ITick* mRenderTickInstance = nullptr;
  ITick* mInputTickInstance = nullptr;
};
