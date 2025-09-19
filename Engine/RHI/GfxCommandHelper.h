#pragma once
#include "Core/Memory/UniquePtr.h"
#include "Core/Performance/ProfilerMark.h"
#include "Core/TaskGraph/TaskHandle.h"
#include "Math/Vector.h"
#include "RHIEnums.h"

class RHIImage;
class RHICommandBuffer;
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

  void WaitAll();

  void WaitCommitFinished() {
    CPU_PROFILING_SCOPE;
    CommitHandle.WaitSync();
  }

  void WaitGpuFinished();

  TaskHandle CommitHandle;
  UniquePtr<RHIFence> GpuExecuteFence;
  /// 这次调用使用的CommandPool
  UniquePtr<RHICommandPool> CommandPool;
  /// 这次调用使用的CommandBuffer
  UniquePtr<RHICommandBuffer> CommandBuffer;
};

class GfxCommandHelper {
public:
  static void Copy(RHIBuffer* Source, RHIBuffer* Dest, const UInt64 Size, const UInt64 SourceOffset, const UInt64 DestOffset) { CopyAsync(Source, Dest, Size, SourceOffset, DestOffset); }
  static GfxCommandSyncHandle CopyAsync(RHIBuffer* Source, RHIBuffer* Dest, UInt64 Size, UInt64 SourceOffset, UInt64 DestOffset);

  static void CopyBufferToImage(RHIBuffer* Source, RHIImage* Dest, const UInt64 BufferOffset = 0, const Vector3i ImageOffset = {}, const Vector3u ImageExtent = {}) {
    CopyBufferToImageAsync(Source, Dest, BufferOffset, ImageOffset, ImageExtent).WaitAll();
  }
  static GfxCommandSyncHandle CopyBufferToImageAsync(RHIBuffer* Source, RHIImage* Dest, UInt64 BufferOffset = 0, Vector3i ImageOffset = {}, Vector3u ImageExtent = {});

  /// 创建一个单次的CommandBuffer
  ///  @param Family
  ///  @return 创建的CommandBuffer已经调用了BeginRecord
  static GfxCommandSyncHandle CreateSingleTimeCommandBuffer(ERHIQueueFamilyType Family = ERHIQueueFamilyType::Graphics);

  /// 对一个CommandBuffer EndRecord并且提交
  /// @param Handle
  static void SubmitSingleTimeCommandBuffer(GfxCommandSyncHandle& Handle);

  /// 对一个CommandBuffer EndRecord并且提交 然后等待所有操作完成
  /// @param Handle
  static void SubmitSingleTimeCommandBufferAndWait(GfxCommandSyncHandle& Handle);

private:
#if KITA_DEBUG
  static inline int sNameCounter = 0;
#endif
};
