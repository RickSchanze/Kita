#pragma once
#include "FrameBuffer.h"
#include "Math/Color.h"
#include "Math/Vector.h"
#include "RHIEnums.h"
#include "RenderPass.h"

class RHIBuffer;

enum class ERHICommandType {
  BeginRenderPass,
  EndRenderPass,
  BeginRecord,
  EndRecord,
  CopyBuffer,
};

struct IRHICommand {
public:
  virtual ~IRHICommand() = default;
  [[nodiscard]] virtual ERHICommandType GetType() const = 0;
};

struct RHICmd_BeginRecord : IRHICommand {
  [[nodiscard]] virtual ERHICommandType GetType() const override { return ERHICommandType::BeginRecord; }

  RHI_DEFINE_BUILDER_FIELD_PTR(RHICommandBuffer*, CmdBuffer, nullptr);
};

struct RHICmd_EndRecord : IRHICommand {
  [[nodiscard]] virtual ERHICommandType GetType() const override { return ERHICommandType::EndRecord; }

  RHI_DEFINE_BUILDER_FIELD_PTR(RHICommandBuffer*, CmdBuffer, nullptr);
};

struct RHICmd_BeginRenderPass : IRHICommand {
  [[nodiscard]] virtual ERHICommandType GetType() const override { return ERHICommandType::BeginRenderPass; }

  RHI_DEFINE_BUILDER_FIELD_PTR(RHIRenderPass*, RenderPass, nullptr);
  RHI_DEFINE_BUILDER_FIELD_PTR(RHIFrameBuffer*, FrameBuffer, nullptr);
  RHI_DEFINE_BUILDER_FIELD(Vector2i, Offset, {});
  RHI_DEFINE_BUILDER_FIELD(Vector2u, Size, {})
  RHI_DEFINE_BUILDER_FIELD(Optional<Color>, ClearColor, {});
  RHI_DEFINE_BUILDER_FIELD(Optional<float>, ClearDepthStencil, 1.0f);
};

struct RHICmd_EndRenderPass : IRHICommand {
  [[nodiscard]] virtual ERHICommandType GetType() const override { return ERHICommandType::EndRenderPass; }
};

struct RHICmd_CopyBuffer : IRHICommand {
  [[nodiscard]] virtual ERHICommandType GetType() const override { return ERHICommandType::CopyBuffer; }

  RHI_DEFINE_BUILDER_FIELD_PTR(RHIBuffer*, Source, nullptr); // necessary
  RHI_DEFINE_BUILDER_FIELD_PTR(RHIBuffer*, Dest, nullptr);   // necessary
  RHI_DEFINE_BUILDER_FIELD(UInt64, Size, 0);                 // necessary
  RHI_DEFINE_BUILDER_FIELD(UInt64, SourceOffset, 0);
  RHI_DEFINE_BUILDER_FIELD(UInt64, DestOffset, 0)
};
