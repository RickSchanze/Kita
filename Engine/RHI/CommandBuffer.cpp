//
// Created by kita on 25-8-23.
//
#include "CommandBuffer.h"

#include "Commands.h"
#include "Core/Assert.h"
#include "Generated/Object/Actor.generated.h"

RHICommandBuffer::~RHICommandBuffer() = default;

void RHICommandBuffer::PushCommand(UniquePtr<IRHICommand>&& Cmd) { mCommandQueue.Enqueue(std::move(Cmd)); }

void RHICommandBuffer::Clear() { mCommandQueue = {}; }

void RHICommandBuffer::BeginRecord() {
  UniquePtr<RHICmd_BeginRecord> Cmd = MakeUnique<RHICmd_BeginRecord>();
  Cmd->SetCmdBuffer(this);
  PushCommand(std::move(Cmd));
}

void RHICommandBuffer::EndRecord() {
  UniquePtr<RHICmd_EndRecord> Cmd = MakeUnique<RHICmd_EndRecord>();
  Cmd->SetCmdBuffer(this);
  PushCommand(std::move(Cmd));
}

void RHICommandBuffer::BeginRenderPass(RHIRenderPass* RenderPass, RHIFrameBuffer* FrameBuffer, const Vector2u Size, const Optional<Color>& ClearColor, const Vector2i Offset,
                                       const Optional<float> ClearDepth) {
  UniquePtr<RHICmd_BeginRenderPass> Cmd = MakeUnique<RHICmd_BeginRenderPass>();
  Cmd->SetRenderPass(RenderPass).SetFrameBuffer(FrameBuffer).SetSize(Size).SetClearColor(ClearColor).SetOffset(Offset).SetClearDepthStencil(ClearDepth);
  PushCommand(std::move(Cmd));
}

void RHICommandBuffer::EndRenderPass() {
  UniquePtr<RHICmd_EndRenderPass> Cmd = MakeUnique<RHICmd_EndRenderPass>();
  PushCommand(std::move(Cmd));
}

void RHICommandBuffer::Copy(RHIBuffer* Source, RHIBuffer* Dest, UInt64 Size, UInt64 SourceOffset, UInt64 DestOffset) {
  ASSERT_MSG(Source != nullptr && Dest != nullptr && Size != 0, "命令CopyBuffer参数配置错误")
  UniquePtr<RHICmd_CopyBuffer> Cmd = MakeUnique<RHICmd_CopyBuffer>();
  Cmd->SetSource(Source).SetDest(Dest).SetSize(Size).SetSourceOffset(SourceOffset).SetDestOffset(DestOffset);
  PushCommand(std::move(Cmd));
}