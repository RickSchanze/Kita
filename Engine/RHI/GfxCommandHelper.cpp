//
// Created by kita on 25-9-3.
//

#include "GfxCommandHelper.h"

#include "Buffer.h"
#include "CommandBuffer.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "GfxContext.h"
#include "Sync.h"

GfxCommandSyncHandle GfxCommandHelper::CopyAsync(RHIBuffer* Source, RHIBuffer* Dest, UInt64 Size, UInt64 SourceOffset, UInt64 DestOffset) {
  GfxCommandSyncHandle Handle;
  Handle.CommandPool = GfxContext::GetRef().CreateCommandPoolU(ERHIQueueFamilyType::Transfer, false);
  Handle.CommandBuffer = Handle.CommandPool->CreateCommandBuffer();
  Handle.CommandBuffer->Copy(Source, Dest, Size, SourceOffset, DestOffset);
  Handle.GpuExecuteFence = GfxContext::GetRef().CreateFenceU();
  Handle.CommitHandle = Handle.CommandBuffer->Execute("");
  return Handle;
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
