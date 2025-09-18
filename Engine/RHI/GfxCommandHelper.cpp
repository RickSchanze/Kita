//
// Created by kita on 25-9-3.
//

#include "GfxCommandHelper.h"

#include "Buffer.h"
#include "CommandBuffer.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "GfxContext.h"
#include "Sync.h"
#include "Vulkan/GfxContext_Vulkan.h"

GfxCommandSyncHandle GfxCommandHelper::CopyAsync(RHIBuffer* Source, RHIBuffer* Dest, const UInt64 Size, const UInt64 SourceOffset, const UInt64 DestOffset) {
  GfxCommandSyncHandle Handle = CreateSingleTimeCommandBuffer(ERHIQueueFamilyType::Transfer);
  Handle.CommandBuffer->Copy(Source, Dest, Size, SourceOffset, DestOffset);
  SubmitSingleTimeCommandBuffer(Handle);
  return Handle;
}

GfxCommandSyncHandle GfxCommandHelper::CopyBufferToImageAsync(RHIBuffer* Source, RHIImage* Dest, const UInt64 BufferOffset, const Vector3i ImageOffset, const Vector3u ImageExtent) {
  GfxCommandSyncHandle Handle = CreateSingleTimeCommandBuffer(ERHIQueueFamilyType::Transfer);
  Handle.CommandBuffer->CopyBufferToImage(Source, Dest, BufferOffset, ImageOffset, ImageExtent);
  SubmitSingleTimeCommandBuffer(Handle);
  return Handle;
}

GfxCommandSyncHandle GfxCommandHelper::CreateSingleTimeCommandBuffer(const ERHIQueueFamilyType Family) {
  GfxCommandSyncHandle Handle;
  Handle.CommandPool = GfxContext::GetRef().CreateCommandPoolU(Family, false);
  Handle.CommandBuffer = Handle.CommandPool->CreateCommandBuffer();
  Handle.CommandBuffer->BeginRecord();
  return Handle;
}

void GfxCommandHelper::SubmitSingleTimeCommandBuffer(GfxCommandSyncHandle& Handle) {
  Handle.CommandBuffer->EndRecord();
  Handle.CommitHandle = Handle.CommandBuffer->Execute("");
#if KITA_DEBUG
  String DebugName = Format("SingleTimeCmdBufferFence_{}", sNameCounter++);
#else
  StringView DebugName = "";
#endif
  Handle.GpuExecuteFence = GfxContext::GetRef().CreateFenceU(DebugName);
  Handle.GpuExecuteFence->Reset();
  RHICommandBufferSubmitParams Params;
  Params.CommandBuffer = Handle.CommandBuffer.Get();
  Params.WaitSemaphores = {};
  Params.SignalSemaphores = {};
  Params.Fence = Handle.GpuExecuteFence.Get();
  Handle.CommitHandle = GfxContext::GetRef().SubmitAsync(Params, {Handle.CommitHandle});
}

void GfxCommandHelper::SubmitSingleTimeCommandBufferAndWait(GfxCommandSyncHandle& Handle) {
  SubmitSingleTimeCommandBuffer(Handle);
  Handle.WaitAll();
}

GfxCommandSyncHandle::GfxCommandSyncHandle() = default;

GfxCommandSyncHandle::~GfxCommandSyncHandle() { WaitAll(); }

void GfxCommandSyncHandle::WaitAll() {
  CPU_PROFILING_SCOPE;
  CommitHandle.WaitSync();
  GpuExecuteFence->Wait(UINT64_MAX);
}

void GfxCommandSyncHandle::WaitGpuFinished() {
  CPU_PROFILING_SCOPE;
  GpuExecuteFence->Wait(UINT64_MAX);
}
