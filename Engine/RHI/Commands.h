#pragma once
#include "FrameBuffer.h"
#include "Math/Color.h"
#include "Math/Vector.h"
#include "RHIEnums.h"
#include "RenderPass.h"

enum class ERHICommandType {
  BeginRenderPass,
  EndRenderPass,
  BeginRecord,
  EndRecord,
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
  RHI_DEFINE_BUILDER_FIELD(Vector2i, Size, {})
  RHI_DEFINE_BUILDER_FIELD(Optional<Color>, ClearColor, {});
  RHI_DEFINE_BUILDER_FIELD(Optional<float>, ClearDepthStencil, 1.0f);
};

struct RHICmd_EndRenderPass : IRHICommand {
  [[nodiscard]] virtual ERHICommandType GetType() const override { return ERHICommandType::EndRenderPass; }
};
