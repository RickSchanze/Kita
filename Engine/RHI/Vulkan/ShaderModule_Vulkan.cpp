//
// Created by kita on 25-8-26.
//

#include "ShaderModule_Vulkan.h"

#include "GfxContext_Vulkan.h"

RHIShaderModule_Vulkan::RHIShaderModule_Vulkan(const RHIShaderModuleDesc& Desc) {
  VkShaderModuleCreateInfo CreateInfo = {};
  CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  CreateInfo.codeSize = Desc.CodeSize;
  CreateInfo.pCode = Desc.Code;
  if (VkResult Result = vkCreateShaderModule(GetVulkanGfxContex()->GetDevice(), &CreateInfo, nullptr, &mModule); Result != VK_SUCCESS) {
    mModule = VK_NULL_HANDLE;
    gLogger.Error("RHI", "创建ShaderModule失败, Code={}", Result);
  }
}

RHIShaderModule_Vulkan::~RHIShaderModule_Vulkan() {
  if (mModule != VK_NULL_HANDLE)
    vkDestroyShaderModule(GetVulkanGfxContex()->GetDevice(), mModule, nullptr);
}