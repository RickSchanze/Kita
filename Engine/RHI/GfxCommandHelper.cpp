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

GfxCommandSyncHandle GfxCommandHelper::CopyAsync(RHIBuffer* Source, RHIBuffer* Dest, UInt64 Size, UInt64 SourceOffset, UInt64 DestOffset) {
  GfxCommandSyncHandle Handle;
  Handle.CommandPool = GfxContext::GetRef().CreateCommandPoolU(ERHIQueueFamilyType::Transfer, false);
  Handle.CommandBuffer = Handle.CommandPool->CreateCommandBuffer();
  Handle.CommandBuffer->BeginRecord();
  Handle.CommandBuffer->Copy(Source, Dest, Size, SourceOffset, DestOffset);
  Handle.CommandBuffer->EndRecord();
  Handle.GpuExecuteFence = GfxContext::GetRef().CreateFenceU();
  Handle.CommitHandle = Handle.CommandBuffer->Execute("");
  RHICommandBufferSubmitParams Params;
  Params.CommandBuffer = Handle.CommandBuffer.Get();
  Params.WaitSemaphores = {};
  Params.SignalSemaphores = {};
  Params.Fence = Handle.GpuExecuteFence.Get();
  Handle.CommitHandle = GfxContext::GetRef().SubmitAsync(Params, {Handle.CommitHandle});
  return Handle;
}

GfxCommandSyncHandle GfxCommandHelper::CreateSingleTimeCommandBuffer(const ERHIQueueFamilyType Family) {
  GfxCommandSyncHandle Handle;
  Handle.CommandPool = GfxContext::GetRef().CreateCommandPoolU(Family, true);
  Handle.CommandBuffer = Handle.CommandPool->CreateCommandBuffer();
  Handle.CommandBuffer->BeginRecord();
  return Handle;
}

void GfxCommandHelper::SubmitSingleTimeCommandBuffer(GfxCommandSyncHandle& Handle) {
  Handle.CommandBuffer->EndRecord();
  Handle.CommitHandle = Handle.CommandBuffer->Execute("");
  Handle.GpuExecuteFence = GfxContext::GetRef().CreateFenceU();
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
