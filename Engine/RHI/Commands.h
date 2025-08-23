#pragma once
#include "FrameBuffer.h"
#include "Math/Color.h"
#include "Math/Vector.h"
#include "RHIFormat.h"
#include "RenderPass.h"

enum class ERHICommandType {
  BeginRenderPass,
  EndRenderPass,
};

struct IRHICommand {
public:
  virtual ~IRHICommand() = default;
  [[nodiscard]] virtual ERHICommandType GetType() const = 0;
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
