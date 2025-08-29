//
// Created by kita on 25-8-29.
//

#include "TickManager.h"

#include "Core/Performance/ProfilerMark.h"

void TickManager::ShutDown() {
  auto& Self = GetRef();
  if (!Self.mTickObjects.Empty()) {
    LOG_WARN_TAG("Tick", "TickManager销毁时, 还有{}个Tick实例未被销毁:", Self.mTickObjects.Count());
    for (auto& Object : Self.mTickObjects) {
#ifdef KITA_DEBUG
      LOG_WARN_TAG("Tick", "'{}': {:p}", Object->GetTickDebugName(), Ptr(Object));
#else
      LOG_WARN_TAG("Tick", "{:p}", Ptr(Object));
#endif
    }
  }
  Self.mTickObjects.Clear();
}

void TickManager::RegisterTickInstanceM(ITick* Instance) {
  if (Instance != nullptr) {
    mTickObjects.AddUnique(Instance);
  }
}

void TickManager::UnRegisterTickInstanceM(ITick* Instance) {
  if (Instance != nullptr) {
    mTickObjects.Remove(Instance);
  }
}

void TickManager::TickAllObjectsM(float DeltaSeconds) {
  ASSERT_MSG(mInputTickInstance != nullptr, "需要设置InputTick实例")
  ASSERT_MSG(mRenderTickInstance != nullptr, "需要设置RenderTick实例")
  CPU_NAMED_PROFILING_SCOPE("TickAllTickInstances");
  {
    CPU_NAMED_PROFILING_SCOPE("TickInput");
    mInputTickInstance->Tick(DeltaSeconds);
  }
  for (const auto TickInstance : mTickObjects) {
    TickInstance->Tick(DeltaSeconds);
  }
  {
    CPU_NAMED_PROFILING_SCOPE("TickRender");
    mRenderTickInstance->Tick(DeltaSeconds);
  }
}