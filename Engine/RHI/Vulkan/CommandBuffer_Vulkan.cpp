//
// Created by kita on 25-8-27.
//

#include "CommandBuffer_Vulkan.h"

#include "Core/TaskGraph/TaskGraph.h"
#include "Core/TaskGraph/TaskNode.h"
#include "GfxContext_Vulkan.h"
#include "RHI/Commands.h"

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
    LOG_ERROR_TAG("RHI.Vulkan", "创建CommandPool失败, 错误码={}", Result);
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
    LOG_ERROR_TAG("RHI.Vulkan", "创建CommandBuffer失败, 错误码={}", Result);
  }
  CmdBuffer->mParentPool = this;
  return CmdBuffer;
}

void RHICommandPool_Vulkan::Reset() { vkResetCommandPool(GetVulkanGfxContexRef().GetDevice(), mPool, 0); }

class ExecuteCommandBufferTask : public TaskNode {
public:
  ExecuteCommandBufferTask(Queue<UniquePtr<IRHICommand>>&& InQueue, VkCommandBuffer InCmdBuffer) : Queue(std::move(InQueue)), CmdBuffer(InCmdBuffer) {}
  virtual ENamedThread GetDesiredThread() const override { return ENamedThread::Render; }

  Queue<UniquePtr<IRHICommand>> Queue;
  VkCommandBuffer CmdBuffer;

  void BeginRecord(IRHICommand* Cmd) {
    VkCommandBufferBeginInfo BeginInfo{};
    BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vkBeginCommandBuffer(CmdBuffer, &BeginInfo);
  }

  void EndRecord(IRHICommand* Cmd) { vkEndCommandBuffer(CmdBuffer); }

  void BeginRenderPass(IRHICommand* Cmd) {
    RHICmd_BeginRenderPass* CmdBeginRenderPass = static_cast<RHICmd_BeginRenderPass*>(Cmd);
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
      float Depth = *CmdBeginRenderPass->ClearDepthStencil;
      ClearValue.depthStencil = {.depth = Depth};
    }
    BeginInfo.pClearValues = &ClearValue;
    vkCmdBeginRenderPass(CmdBuffer, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);
  }

  void EndRenderPass(IRHICommand* Cmd) { vkCmdEndRenderPass(CmdBuffer); }

  virtual ETaskNodeResult Run() override {
    while (!Queue.Empty()) {
      UniquePtr<IRHICommand> Cmd = Queue.Dequeue();
      switch (Cmd->GetType()) {
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
