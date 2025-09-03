#pragma once
#include "Core/Memory/UniquePtr.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/TaskGraph/TaskHandle.h"
#include "Sync.h"

class RHICommandPool;
class RHIBuffer;
class RHIFence;
struct GfxCommandSyncHandle {
  GfxCommandSyncHandle();
  /// 析构函数会同步等待Gpu执行结束
  /// 随后销毁CommandPool
  ~GfxCommandSyncHandle();

  GfxCommandSyncHandle(const GfxCommandSyncHandle&) = delete;
  GfxCommandSyncHandle& operator=(const GfxCommandSyncHandle&) = delete;
  GfxCommandSyncHandle(GfxCommandSyncHandle&& Other) noexcept : CommitHandle(std::move(Other.CommitHandle)), GpuExecuteFence(std::move(Other.GpuExecuteFence)) {}
  GfxCommandSyncHandle& operator=(GfxCommandSyncHandle&& Other) noexcept {
    CommitHandle = std::move(Other.CommitHandle);
    GpuExecuteFence = std::move(Other.GpuExecuteFence);
    return *this;
  }

  void WaitAll() {
    CPU_PROFILING_SCOPE;
    CommitHandle.WaitSync();
    GpuExecuteFence->Wait(UINT64_MAX);
  }

  void WaitCommitFinished() {
    CPU_PROFILING_SCOPE;
    CommitHandle.WaitSync();
  }

  void WaitGpuFinished() {
    CPU_PROFILING_SCOPE;
    GpuExecuteFence->Wait(UINT64_MAX);
  }

  TaskHandle CommitHandle;
  UniquePtr<RHIFence> GpuExecuteFence;
  /// 这次调用使用的CommandPool
  UniquePtr<RHICommandPool> CommandPool;
  /// 这次调用使用的CommandBuffer
  UniquePtr<RHIBuffer> CommandBuffer;
};

class GfxCommandHelper {
public:
  static void Copy(RHIBuffer* Source, RHIBuffer* Dest, UInt64 Size, UInt64 SourceOffset, UInt64 DestOffset) { CopyAsync(Source, Dest, Size, SourceOffset, DestOffset); }
  static GfxCommandSyncHandle CopyAsync(RHIBuffer* Source, RHIBuffer* Dest, UInt64 Size, UInt64 SourceOffset, UInt64 DestOffset);
};
