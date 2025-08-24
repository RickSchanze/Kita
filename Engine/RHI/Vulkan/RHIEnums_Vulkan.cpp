//
// Created by kita on 25-8-24.
//
//
// Created by kita on 25-5-24.
//

#include "RHIEnums_Vulkan.h"

// 实现
VkFormat RHIFormatToVkFormat(const ERHIFormat format) {
  switch (format) {
  case ERHIFormat::R32G32B32_Float:
    return VK_FORMAT_R32G32B32_SFLOAT;
  case ERHIFormat::R32G32B32A32_Float:
    return VK_FORMAT_R32G32B32A32_SFLOAT;
  case ERHIFormat::R16G16B16A16_UNorm:
    return VK_FORMAT_R16G16B16A16_UNORM;
  case ERHIFormat::R32G32_UInt:
    return VK_FORMAT_R32G32_UINT;
  case ERHIFormat::R8G8B8A8_UNorm:
    return VK_FORMAT_R8G8B8A8_UNORM;
  case ERHIFormat::R8G8B8A8_SNorm:
    return VK_FORMAT_R8G8B8A8_SNORM;
  case ERHIFormat::R8G8B8A8_UInt:
    return VK_FORMAT_R8G8B8A8_UINT;
  case ERHIFormat::R8G8B8A8_SInt:
    return VK_FORMAT_R8G8B8A8_SINT;
  case ERHIFormat::D32_Float_S8X24_UInt:
    return VK_FORMAT_D32_SFLOAT_S8_UINT;
  case ERHIFormat::D32_Float:
    return VK_FORMAT_D32_SFLOAT;
  case ERHIFormat::D24_UNorm_S8_UInt:
    return VK_FORMAT_D24_UNORM_S8_UINT;
  case ERHIFormat::D16_UNorm:
    return VK_FORMAT_D16_UNORM;
  case ERHIFormat::B8G8R8A8_SRGB:
    return VK_FORMAT_B8G8R8A8_SRGB;
  case ERHIFormat::A2B10G10R10_UNorm:
    return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
  case ERHIFormat::B8G8R8A8_UNorm:
    return VK_FORMAT_B8G8R8A8_UNORM;
  case ERHIFormat::R16G16B16A16_Float:
    return VK_FORMAT_R16G16B16A16_SFLOAT;
  case ERHIFormat::R32G32_Float:
    return VK_FORMAT_R32G32_SFLOAT;
  case ERHIFormat::R8_UNorm:
    return VK_FORMAT_R8_UNORM;
  case ERHIFormat::R8G8B8A8_SRGB:
    return VK_FORMAT_R8G8B8A8_SRGB;
  case ERHIFormat::R8_SRGB:
    return VK_FORMAT_R8_SRGB;
  case ERHIFormat::R8G8B8_UNorm:
    return VK_FORMAT_R8G8B8_UNORM;
  case ERHIFormat::R32_Float:
    return VK_FORMAT_R32_SFLOAT;
  default:
    return VK_FORMAT_UNDEFINED;
  }
}

ERHIFormat VkFormatToRHIFormat(const VkFormat format) {
  switch (format) {
  case VK_FORMAT_R32G32B32_SFLOAT:
    return ERHIFormat::R32G32B32_Float;
  case VK_FORMAT_R32G32B32A32_SFLOAT:
    return ERHIFormat::R32G32B32A32_Float;
  case VK_FORMAT_R16G16B16A16_UNORM:
    return ERHIFormat::R16G16B16A16_UNorm;
  case VK_FORMAT_R32G32_UINT:
    return ERHIFormat::R32G32_UInt;
  case VK_FORMAT_R8G8B8A8_UNORM:
    return ERHIFormat::R8G8B8A8_UNorm;
  case VK_FORMAT_R8G8B8A8_SNORM:
    return ERHIFormat::R8G8B8A8_SNorm;
  case VK_FORMAT_R8G8B8A8_UINT:
    return ERHIFormat::R8G8B8A8_UInt;
  case VK_FORMAT_R8G8B8A8_SINT:
    return ERHIFormat::R8G8B8A8_SInt;
  case VK_FORMAT_D32_SFLOAT_S8_UINT:
    return ERHIFormat::D32_Float_S8X24_UInt;
  case VK_FORMAT_D32_SFLOAT:
    return ERHIFormat::D32_Float;
  case VK_FORMAT_D24_UNORM_S8_UINT:
    return ERHIFormat::D24_UNorm_S8_UInt;
  case VK_FORMAT_D16_UNORM:
    return ERHIFormat::D16_UNorm;
  case VK_FORMAT_B8G8R8A8_SRGB:
    return ERHIFormat::B8G8R8A8_SRGB;
  case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
    return ERHIFormat::A2B10G10R10_UNorm;
  case VK_FORMAT_B8G8R8A8_UNORM:
    return ERHIFormat::B8G8R8A8_UNorm;
  case VK_FORMAT_R16G16B16A16_SFLOAT:
    return ERHIFormat::R16G16B16A16_Float;
  case VK_FORMAT_R32G32_SFLOAT:
    return ERHIFormat::R32G32_Float;
  case VK_FORMAT_R8_UNORM:
    return ERHIFormat::R8_UNorm;
  case VK_FORMAT_R8G8B8A8_SRGB:
    return ERHIFormat::R8G8B8A8_SRGB;
  case VK_FORMAT_R8_SRGB:
    return ERHIFormat::R8_SRGB;
  case VK_FORMAT_R8G8B8_UNORM:
    return ERHIFormat::R8G8B8_UNorm;
  case VK_FORMAT_R32_SFLOAT:
    return ERHIFormat::R32_Float;
  default:
    return ERHIFormat::Count;
  }
}

VkColorSpaceKHR RHIColorSpaceToVkColorSpace(const ERHIColorSpace ColorSpace) {
  switch (ColorSpace) {
  case ERHIColorSpace::sRGB:
    return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  case ERHIColorSpace::HDR10:
    return VK_COLOR_SPACE_HDR10_ST2084_EXT;
  case ERHIColorSpace::Count:
    return VK_COLOR_SPACE_MAX_ENUM_KHR;
  }
  return VK_COLOR_SPACE_MAX_ENUM_KHR;
}

ERHIColorSpace VkColorSpaceToRHIColorSpace(const VkColorSpaceKHR ColorSpace) {
  switch (ColorSpace) {
  case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
    return ERHIColorSpace::sRGB;
  case VK_COLOR_SPACE_HDR10_ST2084_EXT:
    return ERHIColorSpace::HDR10;
  default:
    return ERHIColorSpace::Count;
  }
}

VkPresentModeKHR RHIPresentModeToVkPresentMode(const ERHIPresentMode present_mode) {
  switch (present_mode) {
  case ERHIPresentMode::Immediate:
    return VK_PRESENT_MODE_IMMEDIATE_KHR;
  case ERHIPresentMode::VSync:
    return VK_PRESENT_MODE_FIFO_KHR;
  case ERHIPresentMode::TripleBuffer:
    return VK_PRESENT_MODE_MAILBOX_KHR;
  case ERHIPresentMode::Count:
    return VK_PRESENT_MODE_MAX_ENUM_KHR;
  }
  return VK_PRESENT_MODE_MAX_ENUM_KHR;
}

ERHIPresentMode VkPresentModeToRHIPresentMode(const VkPresentModeKHR present_mode) {
  switch (present_mode) {
  case VK_PRESENT_MODE_IMMEDIATE_KHR:
    return ERHIPresentMode::Immediate;
  case VK_PRESENT_MODE_FIFO_KHR:
    return ERHIPresentMode::VSync;
  case VK_PRESENT_MODE_MAILBOX_KHR:
    return ERHIPresentMode::TripleBuffer;
  default:
    return ERHIPresentMode::Count;
  }
}

VkSampleCountFlagBits RHISampleCountToVkSampleCount(const ERHISampleCount sample_count) {
  switch (sample_count) {
  case ERHISampleCount::SC_1:
    return VK_SAMPLE_COUNT_1_BIT;
  case ERHISampleCount::SC_2:
    return VK_SAMPLE_COUNT_2_BIT;
  case ERHISampleCount::SC_4:
    return VK_SAMPLE_COUNT_4_BIT;
  case ERHISampleCount::SC_8:
    return VK_SAMPLE_COUNT_8_BIT;
  case ERHISampleCount::SC_16:
    return VK_SAMPLE_COUNT_16_BIT;
  case ERHISampleCount::SC_32:
    return VK_SAMPLE_COUNT_32_BIT;
  case ERHISampleCount::SC_64:
    return VK_SAMPLE_COUNT_64_BIT;
  case ERHISampleCount::SC_Count:
    return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
  default:
    return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
  }
}

ERHISampleCount VkSampleCountToRHISampleCount(const VkSampleCountFlagBits sample_count) {
  switch (sample_count) {
  case VK_SAMPLE_COUNT_1_BIT:
    return ERHISampleCount::SC_1;
  case VK_SAMPLE_COUNT_2_BIT:
    return ERHISampleCount::SC_2;
  case VK_SAMPLE_COUNT_4_BIT:
    return ERHISampleCount::SC_4;
  case VK_SAMPLE_COUNT_8_BIT:
    return ERHISampleCount::SC_8;
  case VK_SAMPLE_COUNT_16_BIT:
    return ERHISampleCount::SC_16;
  case VK_SAMPLE_COUNT_32_BIT:
    return ERHISampleCount::SC_32;
  case VK_SAMPLE_COUNT_64_BIT:
    return ERHISampleCount::SC_64;
  case VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM:
    return ERHISampleCount::SC_Count;
  }
  return ERHISampleCount::SC_Count;
}

VkImageAspectFlags RHIImageAspectToVkImageAspect(const ERHIImageAspect aspect) {
  VkImageAspectFlags flags = 0;
  if (aspect & ERHIImageAspectBits::IA_Color) {
    flags |= VK_IMAGE_ASPECT_COLOR_BIT;
  }
  if (aspect & ERHIImageAspectBits::IA_Depth) {
    flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
  }
  if (aspect & ERHIImageAspectBits::IA_Stencil) {
    flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
  }
  return flags;
}

ERHIImageAspect VkImageAspectToRHIImageAspect(VkImageAspectFlags aspect) {
  int rtn = 0;
  if (rtn & VK_IMAGE_ASPECT_COLOR_BIT) {
    rtn |= ERHIImageAspectBits::IA_Color;
  }
  if (rtn & VK_IMAGE_ASPECT_DEPTH_BIT) {
    rtn |= ERHIImageAspectBits::IA_Depth;
  }
  if (rtn & VK_IMAGE_ASPECT_STENCIL_BIT) {
    rtn |= ERHIImageAspectBits::IA_Stencil;
  }
  return rtn;
}

ERHIComponentMappingElement VkComponentSwizzleToRHIComponentMappingElement(const VkComponentSwizzle swizzle) {
  switch (swizzle) {
  case VK_COMPONENT_SWIZZLE_IDENTITY:
    return ERHIComponentMappingElement::Identity;
  case VK_COMPONENT_SWIZZLE_ZERO:
    return ERHIComponentMappingElement::Zero;
  case VK_COMPONENT_SWIZZLE_ONE:
    return ERHIComponentMappingElement::One;
  case VK_COMPONENT_SWIZZLE_R:
    return ERHIComponentMappingElement::R;
  case VK_COMPONENT_SWIZZLE_G:
    return ERHIComponentMappingElement::G;
  case VK_COMPONENT_SWIZZLE_B:
    return ERHIComponentMappingElement::B;
  case VK_COMPONENT_SWIZZLE_A:
    return ERHIComponentMappingElement::A;
  case VK_COMPONENT_SWIZZLE_MAX_ENUM:
    return ERHIComponentMappingElement::Count;
  }
  return ERHIComponentMappingElement::Count;
}

VkComponentSwizzle RHIComponentMappingElementToVkComponentSwizzle(const ERHIComponentMappingElement swizzle) {
  switch (swizzle) {
  case ERHIComponentMappingElement::Identity:
    return VK_COMPONENT_SWIZZLE_IDENTITY;
  case ERHIComponentMappingElement::Zero:
    return VK_COMPONENT_SWIZZLE_ZERO;
  case ERHIComponentMappingElement::One:
    return VK_COMPONENT_SWIZZLE_ONE;
  case ERHIComponentMappingElement::R:
    return VK_COMPONENT_SWIZZLE_R;
  case ERHIComponentMappingElement::G:
    return VK_COMPONENT_SWIZZLE_G;
  case ERHIComponentMappingElement::B:
    return VK_COMPONENT_SWIZZLE_B;
  case ERHIComponentMappingElement::A:
    return VK_COMPONENT_SWIZZLE_A;
  case ERHIComponentMappingElement::Count:
    return VK_COMPONENT_SWIZZLE_MAX_ENUM;
  }
  return VK_COMPONENT_SWIZZLE_MAX_ENUM;
}

VkImageViewType RHIImageDimensionToVkImageViewType(const ERHIImageDimension type) {
  switch (type) {
  case ERHIImageDimension::D1:
    return VK_IMAGE_VIEW_TYPE_1D;
  case ERHIImageDimension::D2:
    return VK_IMAGE_VIEW_TYPE_2D;
  case ERHIImageDimension::D3:
    return VK_IMAGE_VIEW_TYPE_3D;
  case ERHIImageDimension::Cube:
    return VK_IMAGE_VIEW_TYPE_CUBE;
  case ERHIImageDimension::Array1D:
    return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
  case ERHIImageDimension::Array2D:
    return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
  case ERHIImageDimension::ArrayCube:
    return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
  case ERHIImageDimension::Count:
    return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
  }
  return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
}

ERHIImageDimension VkImageViewTypeToRHIImageDimension(const VkImageViewType type) {
  switch (type) {
  case VK_IMAGE_VIEW_TYPE_1D:
    return ERHIImageDimension::D1;
  case VK_IMAGE_VIEW_TYPE_2D:
    return ERHIImageDimension::D2;
  case VK_IMAGE_VIEW_TYPE_3D:
    return ERHIImageDimension::D3;
  case VK_IMAGE_VIEW_TYPE_CUBE:
    return ERHIImageDimension::Cube;
  case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
    return ERHIImageDimension::Array1D;
  case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
    return ERHIImageDimension::Array2D;
  case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
    return ERHIImageDimension::ArrayCube;
  case VK_IMAGE_VIEW_TYPE_MAX_ENUM:
    return ERHIImageDimension::Count;
  }
  return ERHIImageDimension::Count;
}

VkBufferUsageFlags RHIBufferUsageToVkBufferUsage(const ERHIBufferUsage usage) {
  VkBufferUsageFlags flags = 0;
  if (usage & BUB_VertexBuffer) {
    flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  }
  if (usage & BUB_IndexBuffer) {
    flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  }
  if (usage & BUB_UniformBuffer) {
    flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  }
  if (usage & BUB_TransferSrc) {
    flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  }
  if (usage & BUB_TransferDst) {
    flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  }
  return flags;
}

ERHIBufferUsage VkBufferUsageToRHIBufferUsage(const VkBufferUsageFlags usage) {
  ERHIBufferUsage flags = 0;
  if (usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {
    flags |= BUB_VertexBuffer;
  }
  if (usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT) {
    flags |= BUB_IndexBuffer;
  }
  if (usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
    flags |= BUB_UniformBuffer;
  }
  if (usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) {
    flags |= BUB_TransferSrc;
  }
  if (usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT) {
    flags |= BUB_TransferDst;
  }
  return flags;
}

VkMemoryPropertyFlags RHIMemoryPropertyToVkMemoryProperty(const ERHIBufferMemoryProperty property) {
  VkMemoryPropertyFlags flags = 0;
  if (property & BMPB_DeviceLocal) {
    flags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  }
  if (property & BMPB_HostVisible) {
    flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
  }
  if (property & BMPB_HostCoherent) {
    flags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  }
  return flags;
}

ERHIBufferMemoryProperty VkMemoryPropertyToRHIMemoryProperty(const VkMemoryPropertyFlags property) {
  ERHIBufferMemoryProperty flags = 0;
  if (property & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
    flags |= BMPB_DeviceLocal;
  }
  if (property & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
    flags |= BMPB_HostVisible;
  }
  if (property & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
    flags |= BMPB_HostCoherent;
  }
  return flags;
}

VkImageLayout RHIImageLayoutToVkImageLayout(const ERHIImageLayout layout) {
  switch (layout) {
  case ERHIImageLayout::Undefined:
    return VK_IMAGE_LAYOUT_UNDEFINED;
  case ERHIImageLayout::General:
    return VK_IMAGE_LAYOUT_GENERAL;
  case ERHIImageLayout::ColorAttachment:
    return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  case ERHIImageLayout::DepthStencilAttachment:
    return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  case ERHIImageLayout::TransferSrc:
    return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  case ERHIImageLayout::TransferDst:
    return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  case ERHIImageLayout::ShaderReadOnly:
    return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  case ERHIImageLayout::PresentSrc:
    return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  case ERHIImageLayout::Count:
    return VK_IMAGE_LAYOUT_MAX_ENUM;
  default:
    return VK_IMAGE_LAYOUT_MAX_ENUM;
  }
}

ERHIImageLayout VkImageLayoutToRHIImageLayout(const VkImageLayout layout) {
  switch (layout) {
  case VK_IMAGE_LAYOUT_UNDEFINED:
    return ERHIImageLayout::Undefined;
  case VK_IMAGE_LAYOUT_GENERAL:
    return ERHIImageLayout::General;
  case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
    return ERHIImageLayout::ColorAttachment;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    return ERHIImageLayout::DepthStencilAttachment;
  case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    return ERHIImageLayout::TransferSrc;
  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    return ERHIImageLayout::TransferDst;
  case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
    return ERHIImageLayout::ShaderReadOnly;
  case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
    return ERHIImageLayout::PresentSrc;
  case VK_IMAGE_LAYOUT_MAX_ENUM:
    return ERHIImageLayout::Count;
  default:
    return ERHIImageLayout::Count;
  }
}

VkAttachmentLoadOp RHIAttachmentLoadOpToVkAttachmentLoadOp(const ERHIAttachmentLoadOperation load_op) {
  switch (load_op) {
  case ERHIAttachmentLoadOperation::Load:
    return VK_ATTACHMENT_LOAD_OP_LOAD;
  case ERHIAttachmentLoadOperation::Clear:
    return VK_ATTACHMENT_LOAD_OP_CLEAR;
  case ERHIAttachmentLoadOperation::DontCare:
    return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  default:
    return VK_ATTACHMENT_LOAD_OP_MAX_ENUM;
  }
}

ERHIAttachmentLoadOperation VkAttachmentLoadOpToRHIAttachmentLoadOp(const VkAttachmentLoadOp load_op) {
  switch (load_op) {
  case VK_ATTACHMENT_LOAD_OP_LOAD:
    return ERHIAttachmentLoadOperation::Load;
  case VK_ATTACHMENT_LOAD_OP_CLEAR:
    return ERHIAttachmentLoadOperation::Clear;
  case VK_ATTACHMENT_LOAD_OP_DONT_CARE:
    return ERHIAttachmentLoadOperation::DontCare;
  default:
    return ERHIAttachmentLoadOperation::DontCare;
  }
}

VkAttachmentStoreOp RHIAttachmentStoreOpToVkAttachmentStoreOp(const ERHIAttachmentStoreOperation store_op) {
  switch (store_op) {
  case ERHIAttachmentStoreOperation::Store:
    return VK_ATTACHMENT_STORE_OP_STORE;
  case ERHIAttachmentStoreOperation::DontCare:
    return VK_ATTACHMENT_STORE_OP_DONT_CARE;
  default:
    return VK_ATTACHMENT_STORE_OP_MAX_ENUM;
  }
}

ERHIAttachmentStoreOperation VkAttachmentStoreOpToRHIAttachmentStoreOp(const VkAttachmentStoreOp store_op) {
  switch (store_op) {
  case VK_ATTACHMENT_STORE_OP_STORE:
    return ERHIAttachmentStoreOperation::Store;
  case VK_ATTACHMENT_STORE_OP_DONT_CARE:
    return ERHIAttachmentStoreOperation::DontCare;
  default:
    return ERHIAttachmentStoreOperation::DontCare;
  }
}

VkPolygonMode RHIPolygonModeToVkPolygonMode(const ERHIPolygonMode polygon_mode) {
  switch (polygon_mode) {
  case ERHIPolygonMode::Fill:
    return VK_POLYGON_MODE_FILL;
  case ERHIPolygonMode::Line:
    return VK_POLYGON_MODE_LINE;
  case ERHIPolygonMode::Point:
    return VK_POLYGON_MODE_POINT;
  default:
    return VK_POLYGON_MODE_MAX_ENUM;
  }
}

ERHIPolygonMode VkPolygonModeToRHIPolygonMode(const VkPolygonMode polygon_mode) {
  switch (polygon_mode) {
  case VK_POLYGON_MODE_FILL:
    return ERHIPolygonMode::Fill;
  case VK_POLYGON_MODE_LINE:
    return ERHIPolygonMode::Line;
  case VK_POLYGON_MODE_POINT:
    return ERHIPolygonMode::Point;
  default:
    return ERHIPolygonMode::Count;
  }
}

VkFrontFace RHIFrontFaceToVkFrontFace(const ERHIFrontFace front_face) {
  switch (front_face) {
  case ERHIFrontFace::Clockwise:
    return VK_FRONT_FACE_CLOCKWISE;
  case ERHIFrontFace::CounterClockwise:
    return VK_FRONT_FACE_COUNTER_CLOCKWISE;
  default:
    return VK_FRONT_FACE_MAX_ENUM;
  }
}

ERHIFrontFace VkFrontFaceToRHIFrontFace(const VkFrontFace front_face) {
  switch (front_face) {
  case VK_FRONT_FACE_CLOCKWISE:
    return ERHIFrontFace::Clockwise;
  case VK_FRONT_FACE_COUNTER_CLOCKWISE:
    return ERHIFrontFace::CounterClockwise;
  default:
    return ERHIFrontFace::Count;
  }
}

VkCullModeFlags RHICullModeToVkCullMode(const ERHICullMode cull_mode) {
  switch (cull_mode) {
  case ERHICullMode::Back:
    return VK_CULL_MODE_BACK_BIT;
  case ERHICullMode::Front:
    return VK_CULL_MODE_FRONT_BIT;
  case ERHICullMode::FrontAndBack:
    return VK_CULL_MODE_FRONT_AND_BACK;
  case ERHICullMode::None:
    return VK_CULL_MODE_NONE;
  default:
    return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
  }
}

ERHICullMode VkCullModeToRHICullMode(const VkCullModeFlags cull_mode) {
  switch (cull_mode) {
  case VK_CULL_MODE_BACK_BIT:
    return ERHICullMode::Back;
  case VK_CULL_MODE_FRONT_BIT:
    return ERHICullMode::Front;
  case VK_CULL_MODE_FRONT_AND_BACK:
    return ERHICullMode::FrontAndBack;
  case VK_CULL_MODE_NONE:
    return ERHICullMode::None;
  default:
    return ERHICullMode::Count;
  }
}

VkCompareOp RHICompareOpToVkCompareOp(const ERHICompareOp compare_op) {
  switch (compare_op) {
  case ERHICompareOp::Never:
    return VK_COMPARE_OP_NEVER;
  case ERHICompareOp::Less:
    return VK_COMPARE_OP_LESS;
  case ERHICompareOp::Equal:
    return VK_COMPARE_OP_EQUAL;
  case ERHICompareOp::LessOrEqual:
    return VK_COMPARE_OP_LESS_OR_EQUAL;
  case ERHICompareOp::Greater:
    return VK_COMPARE_OP_GREATER;
  case ERHICompareOp::NotEqual:
    return VK_COMPARE_OP_NOT_EQUAL;
  case ERHICompareOp::GreaterOrEqual:
    return VK_COMPARE_OP_GREATER_OR_EQUAL;
  case ERHICompareOp::Always:
    return VK_COMPARE_OP_ALWAYS;
  default:
    return VK_COMPARE_OP_MAX_ENUM;
  }
}

ERHICompareOp VkCompareOpToRHICompareOp(const VkCompareOp compare_op) {
  switch (compare_op) {
  case VK_COMPARE_OP_NEVER:
    return ERHICompareOp::Never;
  case VK_COMPARE_OP_LESS:
    return ERHICompareOp::Less;
  case VK_COMPARE_OP_EQUAL:
    return ERHICompareOp::Equal;
  case VK_COMPARE_OP_LESS_OR_EQUAL:
    return ERHICompareOp::LessOrEqual;
  case VK_COMPARE_OP_GREATER:
    return ERHICompareOp::Greater;
  case VK_COMPARE_OP_NOT_EQUAL:
    return ERHICompareOp::NotEqual;
  case VK_COMPARE_OP_GREATER_OR_EQUAL:
    return ERHICompareOp::GreaterOrEqual;
  case VK_COMPARE_OP_ALWAYS:
    return ERHICompareOp::Always;
  default:
    return ERHICompareOp::Count;
  }
}

VkShaderStageFlags RHIShaderStageToVkShaderStage(const ERHIShaderStage stage) {
  VkShaderStageFlags flags = 0;
  if (stage & ERHIShaderStageBits::SSB_Vertex) {
    flags |= VK_SHADER_STAGE_VERTEX_BIT;
  }
  if (stage & ERHIShaderStageBits::SSB_Fragment) {
    flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
  }
  if (stage & ERHIShaderStageBits::SSB_Compute) {
    flags |= VK_SHADER_STAGE_COMPUTE_BIT;
  }
  return flags;
}

ERHIShaderStage VkShaderStageToRHIShaderStage(const VkShaderStageFlags stage) {
  ERHIShaderStage flags = 0;
  if (stage & VK_SHADER_STAGE_VERTEX_BIT) {
    flags |= ERHIShaderStageBits::SSB_Vertex;
  }
  if (stage & VK_SHADER_STAGE_FRAGMENT_BIT) {
    flags |= ERHIShaderStageBits::SSB_Fragment;
  }
  if (stage & VK_SHADER_STAGE_COMPUTE_BIT) {
    flags |= ERHIShaderStageBits::SSB_Compute;
  }
  return flags;
}

VkShaderStageFlagBits RHIShaderStageBitToVkShaderStageBit(const ERHIShaderStageBits Stage) {
  switch (Stage) {
  case ERHIShaderStageBits::SSB_Vertex:
    return VK_SHADER_STAGE_VERTEX_BIT;
  case ERHIShaderStageBits::SSB_Fragment:
    return VK_SHADER_STAGE_FRAGMENT_BIT;
  case ERHIShaderStageBits::SSB_Compute:
    return VK_SHADER_STAGE_COMPUTE_BIT;
  default:
    return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
  }
}

VkDescriptorType RHIDescriptorTypeToVkDescriptorType(const ERHIDescriptorType type) {
  switch (type) {
  case ERHIDescriptorType::Sampler:
    return VK_DESCRIPTOR_TYPE_SAMPLER;
  // case DescriptorType::CombinedImageSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  case ERHIDescriptorType::SampledImage:
    return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
  case ERHIDescriptorType::StorageImage:
    return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
  // case DescriptorType::UniformTexelBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
  // case DescriptorType::StorageTexelBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
  case ERHIDescriptorType::UniformBuffer:
    return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  // case DescriptorType::StorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  // case DescriptorType::StorageBufferDynamic: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
  default:
    return VK_DESCRIPTOR_TYPE_MAX_ENUM;
  }
}

ERHIDescriptorType VkDescriptorTypeToRHIDescriptorType(const VkDescriptorType type) {
  switch (type) {
  case VK_DESCRIPTOR_TYPE_SAMPLER:
    return ERHIDescriptorType::Sampler;
  // case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return DescriptorType::CombinedImageSampler;
  case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
    return ERHIDescriptorType::SampledImage;
  case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
    return ERHIDescriptorType::StorageImage;
  // case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return DescriptorType::UniformTexelBuffer;
  // case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return DescriptorType::StorageTexelBuffer;
  case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
    return ERHIDescriptorType::UniformBuffer;
  // case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: return DescriptorType::StorageBuffer;
  // case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return DescriptorType::StorageBufferDynamic;
  default:
    return ERHIDescriptorType::Count;
  }
}

VkAccessFlags RHIAccessFlagToVkAccessFlag(const ERHIAccessFlags access_flag) {
  if (access_flag == 0)
    return 0;
  VkAccessFlags flags = 0;
  if (access_flag & AFB_ColorAttachmentRead)
    flags |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
  if (access_flag & AFB_ColorAttachmentWrite)
    flags |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  if (access_flag & AFB_DepthStencilAttachmentRead)
    flags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
  if (access_flag & AFB_DepthStencilAttachmentWrite)
    flags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  if (access_flag & AFB_TransferRead)
    flags |= VK_ACCESS_TRANSFER_READ_BIT;
  if (access_flag & AFB_TransferWrite)
    flags |= VK_ACCESS_TRANSFER_WRITE_BIT;
  if (access_flag & AFB_ShaderRead)
    flags |= VK_ACCESS_SHADER_READ_BIT;
  if (access_flag & AFB_ShaderWrite)
    flags |= VK_ACCESS_SHADER_WRITE_BIT;
  return flags;
}

ERHIAccessFlags VkAccessFlagToRHIAccessFlag(const VkAccessFlags access_flag) {
  if (access_flag == 0)
    return 0;
  ERHIAccessFlags flags = 0;
  if (access_flag & VK_ACCESS_COLOR_ATTACHMENT_READ_BIT)
    flags |= AFB_ColorAttachmentRead;
  if (access_flag & VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
    flags |= AFB_ColorAttachmentWrite;
  if (access_flag & VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT)
    flags |= AFB_DepthStencilAttachmentRead;
  if (access_flag & VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT)
    flags |= AFB_DepthStencilAttachmentWrite;
  if (access_flag & VK_ACCESS_TRANSFER_READ_BIT)
    flags |= AFB_TransferRead;
  if (access_flag & VK_ACCESS_TRANSFER_WRITE_BIT)
    flags |= AFB_TransferWrite;
  if (access_flag & VK_ACCESS_SHADER_READ_BIT)
    flags |= AFB_ShaderRead;
  return flags;
}

VkPipelineStageFlags RHIPipelineStageToVkPipelineStage(const ERHIPipelineStageFlags pipeline_stage) {
  if (pipeline_stage == 0)
    return 0;
  VkPipelineStageFlags flags = 0;
  if (pipeline_stage & PSFB_ColorAttachmentOutput)
    flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  if (pipeline_stage & PSFB_TopOfPipe)
    flags |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
  if (pipeline_stage & PSFB_BottomOfPipe)
    flags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  if (pipeline_stage & PSFB_DepthStencilAttachment)
    flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  if (pipeline_stage & PSFB_Transfer)
    flags |= VK_PIPELINE_STAGE_TRANSFER_BIT;
  if (pipeline_stage & PSFB_FragmentShader)
    flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  if (pipeline_stage & PSFB_ComputeShader)
    flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
  return flags;
}

ERHIPipelineStageFlags VkPipelineStageToRHIPipelineStage(const VkPipelineStageFlags pipeline_stage) {
  if (pipeline_stage == 0)
    return 0;
  ERHIPipelineStageFlags flags = 0;
  if (pipeline_stage & VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
    flags |= PSFB_ColorAttachmentOutput;
  if (pipeline_stage & VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT)
    flags |= PSFB_TopOfPipe;
  if (pipeline_stage & VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT)
    flags |= PSFB_BottomOfPipe;
  if (pipeline_stage & VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
    flags |= PSFB_DepthStencilAttachment;
  if (pipeline_stage & VK_PIPELINE_STAGE_TRANSFER_BIT)
    flags |= PSFB_Transfer;
  if (pipeline_stage & VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)
    flags |= PSFB_FragmentShader;
  return flags;
}

VkVertexInputRate RHIVertexInputRateToVkVertexInputRate(const ERHIVertexInputRate rate) {
  switch (rate) {
  case ERHIVertexInputRate::Vertex:
    return VK_VERTEX_INPUT_RATE_VERTEX;
  case ERHIVertexInputRate::Instance:
    return VK_VERTEX_INPUT_RATE_INSTANCE;
  default:
    return VK_VERTEX_INPUT_RATE_MAX_ENUM;
  }
}

ERHIVertexInputRate VkVertexInputRateToRHIVertexInputRate(const VkVertexInputRate rate) {
  switch (rate) {
  case VK_VERTEX_INPUT_RATE_VERTEX:
    return ERHIVertexInputRate::Vertex;
  case VK_VERTEX_INPUT_RATE_INSTANCE:
    return ERHIVertexInputRate::Instance;
  default:
    return ERHIVertexInputRate::Count;
  }
}

VkImageType RHIImageDimensionToVkImageType(const ERHIImageDimension dimension) {
  switch (dimension) {
  case ERHIImageDimension::D1:
    return VK_IMAGE_TYPE_1D;
  case ERHIImageDimension::D2:
    return VK_IMAGE_TYPE_2D;
  case ERHIImageDimension::D3:
    return VK_IMAGE_TYPE_3D;
  default:
    return VK_IMAGE_TYPE_MAX_ENUM;
  }
}

ERHIImageDimension VkImageTypeToRHIImageDimension(const VkImageType type) {
  switch (type) {
  case VK_IMAGE_TYPE_1D:
    return ERHIImageDimension::D1;
  case VK_IMAGE_TYPE_2D:
    return ERHIImageDimension::D2;
  case VK_IMAGE_TYPE_3D:
    return ERHIImageDimension::D3;
  default:
    return ERHIImageDimension::Count;
  }
}

VkImageUsageFlags RHIImageUsageToVkImageUsageFlags(const ERHIImageUsage usage_flag) {
  VkImageUsageFlags flags = 0;
  if (usage_flag & IUB_TransferSrc)
    flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
  if (usage_flag & IUB_TransferDst)
    flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  if (usage_flag & IUB_DepthStencil)
    flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  if (usage_flag & IUB_Transient)
    flags |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
  if (usage_flag & IUB_RenderTarget)
    flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  if (usage_flag & IUB_ShaderRead)
    flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
  if (usage_flag & IUB_Storage)
    flags |= VK_IMAGE_USAGE_STORAGE_BIT;
  return flags;
}

VkFilter RHIFilterToVkFilter(const ERHIFilterMode filter) {
  switch (filter) {
  case ERHIFilterMode::Linear:
    return VK_FILTER_LINEAR;
  case ERHIFilterMode::Nearest:
    return VK_FILTER_NEAREST;
  default:
    return VK_FILTER_MAX_ENUM;
  }
}

ERHIFilterMode VkFilterToRHIFilter(const VkFilter filter) {
  switch (filter) {
  case VK_FILTER_LINEAR:
    return ERHIFilterMode::Linear;
  case VK_FILTER_NEAREST:
    return ERHIFilterMode::Nearest;
  default:
    return ERHIFilterMode::Count;
  }
}

VkSamplerAddressMode RHISamplerAddressModeToVkSamplerAddressMode(const ERHISamplerAddressMode address_mode) {
  switch (address_mode) {
  case ERHISamplerAddressMode::Repeat:
    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
  case ERHISamplerAddressMode::MirroredRepeat:
    return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
  case ERHISamplerAddressMode::ClampToEdge:
    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  default:
    return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
  }
}

ERHISamplerAddressMode VkSamplerAddressModeToRHISamplerAddressMode(const VkSamplerAddressMode address_mode) {
  switch (address_mode) {
  case VK_SAMPLER_ADDRESS_MODE_REPEAT:
    return ERHISamplerAddressMode::Repeat;
  case VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:
    return ERHISamplerAddressMode::MirroredRepeat;
  case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:
    return ERHISamplerAddressMode::ClampToEdge;
  default:
    return ERHISamplerAddressMode::Count;
  }
}

VkBlendFactor RHIBlendFactorToVkBlendFactor(const ERHIBlendFactor blend_factor) {
  switch (blend_factor) {
  case ERHIBlendFactor::Zero:
    return VK_BLEND_FACTOR_ZERO;
  case ERHIBlendFactor::One:
    return VK_BLEND_FACTOR_ONE;
  case ERHIBlendFactor::SrcAlpha:
    return VK_BLEND_FACTOR_SRC_ALPHA;
  case ERHIBlendFactor::OneMinusSrcAlpha:
    return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  default:
    return VK_BLEND_FACTOR_MAX_ENUM;
  }
}

ERHIBlendFactor VkBlendFactorToRHIBlendFactor(const VkBlendFactor blend_factor) {
  switch (blend_factor) {
  case VK_BLEND_FACTOR_ZERO:
    return ERHIBlendFactor::Zero;
  case VK_BLEND_FACTOR_ONE:
    return ERHIBlendFactor::One;
  case VK_BLEND_FACTOR_SRC_ALPHA:
    return ERHIBlendFactor::SrcAlpha;
  case VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
    return ERHIBlendFactor::OneMinusSrcAlpha;
  default:
    return ERHIBlendFactor::Count;
  }
}

ERHIImageUsage VkImageUsageToRHIImageUsageFlags(const VkImageUsageFlags usage_flag) {
  ERHIImageUsage flags = 0;
  if (usage_flag & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
    flags |= IUB_TransferSrc;
  if (usage_flag & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
    flags |= IUB_TransferDst;
  if (usage_flag & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
    flags |= IUB_DepthStencil;
  if (usage_flag & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)
    flags |= IUB_Transient;
  if (usage_flag & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
    flags |= IUB_RenderTarget;
  if (usage_flag & VK_IMAGE_USAGE_SAMPLED_BIT)
    flags |= IUB_ShaderRead;
  return flags;
}

VkBlendOp RHIBlendOpToVkBlendOp(const ERHIBlendOp op) {
  switch (op) {
  case ERHIBlendOp::Add:
    return VK_BLEND_OP_ADD;
  default:
    return VK_BLEND_OP_MAX_ENUM;
  }
}

ERHIBlendOp VKBlendOpToRHIBlendOp(const VkBlendOp op) {
  switch (op) {
  case VK_BLEND_OP_ADD:
    return ERHIBlendOp::Add;
  default:
    return ERHIBlendOp::Count;
  }
}

VkSurfaceTransformFlagsKHR RHISurfaceTransformToVkSurfaceTransform(const ERHISurfaceTransform Transform) {
  VkSurfaceTransformFlagsKHR Result = 0;
  if (Transform | STFB_Identity)
    Result |= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  if (Transform | STFB_Rotate90)
    Result |= VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;
  if (Transform | STFB_Rotate180)
    Result |= VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;
  if (Transform | STFB_Rotate270)
    Result |= VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;
  if (Transform | STFB_HorizontalMirror)
    Result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR;
  if (Transform | STFB_HorizontalMirrorRotate90)
    Result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR;
  if (Transform | STFB_HorizontalMirrorRotate180)
    Result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR;
  if (Transform | STFB_HorizontalMirrorRotate270)
    Result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;
  if (Transform | STFB_Inherit)
    Result |= VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR;
  return Result;
}

ERHISurfaceTransform VkSurfaceTransformToRHISurfaceTransform(const VkSurfaceTransformFlagsKHR Transform) {
  ERHISurfaceTransform Result = 0;
  if (Transform & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    Result |= STFB_Identity;
  if (Transform & VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR)
    Result |= STFB_Rotate90;
  if (Transform & VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR)
    Result |= STFB_Rotate180;
  if (Transform & VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR)
    Result |= STFB_Rotate270;
  if (Transform & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR)
    Result |= STFB_HorizontalMirror;
  if (Transform & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR)
    Result |= STFB_HorizontalMirrorRotate90;
  if (Transform & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR)
    Result |= STFB_HorizontalMirrorRotate180;
  if (Transform & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR)
    Result |= STFB_HorizontalMirrorRotate270;
  if (Transform & VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR)
    Result |= STFB_Inherit;
  return Result;
}

VkPrimitiveTopology RHIPrimitiveTopologyToVkPrimitiveTopology(const ERHIPrimitiveTopology Topology) {
  switch (Topology) {
  case ERHIPrimitiveTopology::TriangleList:
    return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  default:
    return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
  }
}

VkLogicOp RHILogicOpToVkLogicOp(const ERHILogicOp op) {
  switch (op) {
  case ERHILogicOp::Copy:
    return VK_LOGIC_OP_COPY;
  default:
    return VK_LOGIC_OP_MAX_ENUM;
  }
}

VkColorComponentFlags RHIColorComponentToVkColorComponent(const ERHIColorComponent Component) {
  VkColorComponentFlags flags = 0;
  if (Component & ECCB_R)
    flags |= VK_COLOR_COMPONENT_R_BIT;
  if (Component & ECCB_G)
    flags |= VK_COLOR_COMPONENT_G_BIT;
  if (Component & ECCB_B)
    flags |= VK_COLOR_COMPONENT_B_BIT;
  if (Component & ECCB_A)
    flags |= VK_COLOR_COMPONENT_A_BIT;
  return flags;
}
