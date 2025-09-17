//
// Created by kita on 25-8-27.
//

#include "CommandBuffer_Vulkan.h"

#include "Core/Performance/ProfilerMark.h"
#include "Core/TaskGraph/TaskGraph.h"
#include "Core/TaskGraph/TaskNode.h"
#include "GfxContext_Vulkan.h"
#include "RHI/Buffer.h"
#include "RHI/Commands.h"
#include "RHI/Image.h"
#include "RHIEnums_Vulkan.h"

RHICommandPool_Vulkan::RHICommandPool_Vulkan(ERHIQueueFamilyType Family, bool AllowReset) {
  const auto& Ctx = GetVulkanGfxContexRef();
  const uint32_t FamilyIndex = Ctx.GetQueueFamilyIndex(Family);
  VkCommandPoolCreateInfo PoolInfo{};
  PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  PoolInfo.queueFamilyIndex = FamilyIndex;
  if (AllowReset) {
    PoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  }
  if (const VkResult Result = vkCreateCommandPool(Ctx.GetDevice(), &PoolInfo, nullptr, &mPool); Result != VK_SUCCESS) {
    gLogger.Error("RHI", "创建CommandPool失败, 错误码={}", Result);
  }
  mFamily = Family;
}

RHICommandPool_Vulkan::~RHICommandPool_Vulkan() {
  vkDestroyCommandPool(GetVulkanGfxContexRef().GetDevice(), mPool, nullptr);
  mPool = nullptr;
}

UniquePtr<RHICommandBuffer> RHICommandPool_Vulkan::CreateCommandBuffer() {
  UniquePtr<RHICommandBuffer_Vulkan> CmdBuffer = MakeUnique<RHICommandBuffer_Vulkan>();
  VkCommandBufferAllocateInfo AllocInfo{};
  AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  AllocInfo.commandBufferCount = 1;
  AllocInfo.commandPool = mPool;
  AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  if (const VkResult Result = vkAllocateCommandBuffers(GetVulkanGfxContexRef().GetDevice(), &AllocInfo, &CmdBuffer->mBuffer); Result != VK_SUCCESS) {
    gLogger.Error("RHI", "创建CommandBuffer失败, 错误码={}", Result);
  }
  CmdBuffer->mParentPool = this;
  return CmdBuffer;
}

void RHICommandPool_Vulkan::Reset() { vkResetCommandPool(GetVulkanGfxContexRef().GetDevice(), mPool, 0); }

struct VulkanStateInfo {
  VkImageLayout layout;
  VkPipelineStageFlags stageMask;
  VkAccessFlags accessMask;
};

static const VulkanStateInfo StateTable[] = {
    /* Undefined */
    {VK_IMAGE_LAYOUT_UNDEFINED, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0},
    /* RenderTarget */
    {VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT},
    /* DepthWrite */
    {VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT},
    /* DepthRead */
    {VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT},
    /* ShaderResource */
    {VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_ACCESS_SHADER_READ_BIT},
    /* CopySrc */
    {VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_READ_BIT},
    /* CopyDst */
    {VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT},
    /* Present */
    {VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0}};

static Int32 GetStateTableIndex(VkImageLayout Layout) {
  switch (Layout) {
  case VK_IMAGE_LAYOUT_UNDEFINED:
    return 0;
  case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
    return 1;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    return 2;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
    return 3;
  case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
    return 4;
  case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    return 5;
  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    return 6;
  case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
    return 7;
  case VK_IMAGE_LAYOUT_MAX_ENUM:
    return -1;
  default:
    return -1;
  }
}

class ExecuteCommandBufferTask : public TaskNode {
public:
  ExecuteCommandBufferTask(Queue<UniquePtr<IRHICommand>>&& InQueue, VkCommandBuffer InCmdBuffer) : Queue(std::move(InQueue)), CmdBuffer(InCmdBuffer) {}
  virtual ENamedThread GetDesiredThread() const override { return ENamedThread::Render; }

  Queue<UniquePtr<IRHICommand>> Queue;
  VkCommandBuffer CmdBuffer;

  void BeginRecord(IRHICommand* Cmd) const {
    CPU_PROFILING_SCOPE;
    VkCommandBufferBeginInfo BeginInfo{};
    BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vkBeginCommandBuffer(CmdBuffer, &BeginInfo);
  }

  void EndRecord(IRHICommand* Cmd) const {
    CPU_PROFILING_SCOPE;
    vkEndCommandBuffer(CmdBuffer);
  }

  void BeginRenderPass(IRHICommand* Cmd) const {
    CPU_PROFILING_SCOPE;
    const auto CmdBeginRenderPass = static_cast<RHICmd_BeginRenderPass*>(Cmd);
    VkRenderPassBeginInfo BeginInfo{};
    BeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    BeginInfo.renderPass = CmdBeginRenderPass->RenderPass->GetNativeHandleT<VkRenderPass>();
    BeginInfo.framebuffer = CmdBeginRenderPass->FrameBuffer->GetNativeHandleT<VkFramebuffer>();
    BeginInfo.renderArea = {.offset = {CmdBeginRenderPass->Offset.X(), CmdBeginRenderPass->Offset.Y()}, .extent = {CmdBeginRenderPass->Size.X(), CmdBeginRenderPass->Size.Y()}};
    BeginInfo.clearValueCount = 1;
    VkClearValue ClearValue{};
    if (CmdBeginRenderPass->ClearColor) {
      Color ClearColor = *CmdBeginRenderPass->ClearColor;
      ClearValue.color = {.float32 = {ClearColor.R(), ClearColor.G(), ClearColor.B(), ClearColor.A()}};
    }
    if (CmdBeginRenderPass->ClearDepthStencil) {
      // TODO: 加上Stencil
      const float Depth = *CmdBeginRenderPass->ClearDepthStencil;
      ClearValue.depthStencil = {.depth = Depth};
    }
    BeginInfo.pClearValues = &ClearValue;
    vkCmdBeginRenderPass(CmdBuffer, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);
  }

  void EndRenderPass(IRHICommand* Cmd) const {
    CPU_PROFILING_SCOPE;
    vkCmdEndRenderPass(CmdBuffer);
  }

  void CopyBuffer(IRHICommand* Cmd) const {
    CPU_PROFILING_SCOPE;
    const auto CmdCopyBuffer = static_cast<RHICmd_CopyBuffer*>(Cmd);
    VkBufferCopy CopyRegion{};
    CopyRegion.size = CmdCopyBuffer->Size;
    CopyRegion.srcOffset = CmdCopyBuffer->SourceOffset;
    CopyRegion.dstOffset = CmdCopyBuffer->DestOffset;
    vkCmdCopyBuffer(CmdBuffer, CmdCopyBuffer->Source->GetNativeHandleT<VkBuffer>(), CmdCopyBuffer->Dest->GetNativeHandleT<VkBuffer>(), 1, &CopyRegion);
  }

  void ResourceBarrier(IRHICommand* Cmd) const {
    const auto CmdResourceBarrier = static_cast<RHICmd_ResourceBarrier*>(Cmd);
    auto& Src = StateTable[GetStateTableIndex(RHIImageLayoutToVkImageLayout(CmdResourceBarrier->OldLayout))];
    auto& Dst = StateTable[GetStateTableIndex(RHIImageLayoutToVkImageLayout(CmdResourceBarrier->NewLayout))];
    VkImageMemoryBarrier Barrier{};
    Barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    Barrier.oldLayout = Src.layout;
    Barrier.newLayout = Dst.layout;
    Barrier.srcAccessMask = Src.accessMask;
    Barrier.dstAccessMask = Dst.accessMask;
    Barrier.image = CmdResourceBarrier->Image->GetNativeHandleT<VkImage>();
    if (True(CmdResourceBarrier->Image->GetDesc().Usage | ERHIImageUsage::DepthStencil)) {
      Barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    if (True(CmdResourceBarrier->Image->GetDesc().Usage | ERHIImageUsage::ShaderRead)) {
      Barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }
    Barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    Barrier.subresourceRange.baseArrayLayer = 0;
    Barrier.subresourceRange.layerCount = CmdResourceBarrier->Image->GetDesc().GetArrayLayers();
    Barrier.subresourceRange.baseMipLevel = 0;
    Barrier.subresourceRange.levelCount = CmdResourceBarrier->Image->GetDesc().GetMipLevels();
    vkCmdPipelineBarrier(CmdBuffer, Src.stageMask, Dst.stageMask, 0, 0, nullptr, 0, nullptr, 1, &Barrier);
  }

  virtual ETaskNodeResult Run() override {
    CPU_PROFILING_SCOPE;
    while (!Queue.Empty()) {
      switch (UniquePtr<IRHICommand> Cmd = Queue.Dequeue(); Cmd->GetType()) {
      case ERHICommandType::BeginRecord:
        BeginRecord(Cmd.Get());
        break;
      case ERHICommandType::EndRecord:
        EndRecord(Cmd.Get());
        break;
      case ERHICommandType::BeginRenderPass:
        BeginRenderPass(Cmd.Get());
        break;
      case ERHICommandType::EndRenderPass:
        EndRenderPass(Cmd.Get());
        break;
      case ERHICommandType::CopyBuffer:
        CopyBuffer(Cmd.Get());
        break;
      default:
        break;
      }
    }
    return ETaskNodeResult::Success;
  }
};

TaskHandle RHICommandBuffer_Vulkan::Execute(StringView DebugName) {
  const TaskHandle Return = TaskGraph::CreateTask<ExecuteCommandBufferTask>("", {}, std::move(mCommandQueue), mBuffer);
  mCommandQueue = {};
  return Return;
}

void RHICommandBuffer_Vulkan::Reset() { vkResetCommandBuffer(mBuffer, 0); }
