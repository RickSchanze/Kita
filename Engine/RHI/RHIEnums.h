#pragma once
#include "Core/Reflection/MetaMark.h"
#include "Core/TypeDefs.h"

#include "RHIEnums.generated.h"

#define RHI_DEFINE_BUILDER_FIELD(Type, Name, DefaultValue)   \
  Type Name = DefaultValue;                                  \
  FORCE_INLINE Type Get##Name() const { return this->Name; } \
  FORCE_INLINE auto& Set##Name(const Type& Value) {          \
    this->Name = Value;                                      \
    return *this;                                            \
  }

#define RHI_DEFINE_BUILDER_FIELD_PTR(Type, Name, DefaultValue)           \
  static_assert(std::is_pointer<Type>::value, "Type must be a pointer"); \
  Type Name = DefaultValue;                                              \
  FORCE_INLINE Type Get##Name() const { return this->Name; }             \
  FORCE_INLINE auto& Set##Name(Type Value) {                             \
    this->Name = Value;                                                  \
    return *this;                                                        \
  }

KENUM()
enum class ERHIFormat {
  R32G32B32_Float,    // 每个元素由3个32位浮点数分量构成
  R32G32B32A32_Float, // 每个元素由4个32位浮点数分量构成
  R8G8B8_UNorm,
  R16G16B16A16_UNorm,   // 每个元素由4个16位分量构成, 每个分量被映射到[0, 1]
  R32G32_UInt,          // 每个元素由2个32位无符号整数构成
  R8G8B8A8_UNorm,       // 每个元素由4个8位无符号数构成, 被映射到[0, 1]
  R8G8B8A8_SNorm,       // 每个元素由4个8位有符号数构成, 被映射到[-1, 1]
  R8G8B8A8_UInt,        // 每个元素由4个8位无符号数构成, 被映射到[0, 255]
  R8G8B8A8_SInt,        // 每个元素由4个8位有符号数构成, 被映射到[-128, 127]
  R8G8B8A8_SRGB,        // 每个元素由4个8位分量构成, 被映射到sRGB
  D32_Float_S8X24_UInt, // 深度/模版: 64位 32位给深度, 8位给模版([0, 255]), 24位用作对齐
  D32_Float,            // 深度: 32位浮点数
  D24_UNorm_S8_UInt,    // 深度/模版: 24位给深度([0, 1]), 8位给模版([0, 255])
  D16_UNorm,            // 深度: 16位([0, 1])
  B8G8R8A8_SRGB,        // 每个元素由4个8位分量构成, 被映射到sRGB
  B8G8R8A8_UNorm,       // 每个元素由4个8位分量构成, 被映射到[0, 1]
  R16G16B16A16_Float,   // 每个元素由4个16位float分量构成
  A2B10G10R10_UNorm,    // a: 2位 b: 10位 g: 10位 r: 10位
  R32G32_Float,         // 每个元素由2个32位float分量构成
  R32_Float,
  R8_UNorm,
  R8_SRGB,
  Count, // 超出范围(Undefined)
};

KENUM()
enum class ERHIColorSpace {
  sRGB,  // 非线性sRGB, 适用于大部分普通内容, 标准显示器
  HDR10, // HDR10
  Count, // 超出范围
};

KENUM()
enum class ERHIPresentMode {
  VSync,        // 垂直同步
  Immediate,    // 立即刷新
  TripleBuffer, // 三重缓冲
  Count,        // 超出范围
};

enum class ERHIQueueFamilyType {
  Graphics,
  Compute,
  Transfer,
  Present,
};

/**
 * MSAA采样次数
 */
enum class ERHISampleCount {
  SC_1 = 0b0000001,  // 1次
  SC_2 = 0b0000010,  // 2次
  SC_4 = 0b0000100,  // 4次
  SC_8 = 0b0001000,  // 8次
  SC_16 = 0b0010000, // 16次
  SC_32 = 0b0100000, // 32次
  SC_64 = 0b1000000, // 64次
  SC_Count,          // 超出范围
};

// 指示要访问图像的哪些"方面"
enum class ERHIImageAspect {
  None = 0,
  Color = 0b1,     // 颜色
  Depth = 0b10,    // 深度
  Stencil = 0b100, // 模板
  Max,
};
ENABLE_BITMASK_OPERATORS(ERHIImageAspect)

enum class ERHIBufferUsage {
  None = 0,
  VertexBuffer = 1,
  IndexBuffer = 1 << 1,
  UniformBuffer = 1 << 2,
  TransferSrc = 1 << 3,
  TransferDst = 1 << 4,
};
ENABLE_BITMASK_OPERATORS(ERHIBufferUsage)

enum class ERHIBufferMemoryProperty {
  None,
  DeviceLocal = 1,       // GPU Only CPU无法访问(纹理、IndexBuffer、VertexBuffer)
  HostVisible = 1 << 1,  // CPU可以访问(UniformBuffer) 通常和 HostCoherent 一起使用
  HostCoherent = 1 << 2, // CPU可以访问, 但GPU会自动刷新(UniformBuffer)而不用手动同步
  Max = 0x7FFFFFFF,
};
ENABLE_BITMASK_OPERATORS(ERHIBufferMemoryProperty)

enum class ERHIComponentMappingElement {
  Identity,
  Zero,
  One,
  R,
  G,
  B,
  A,
  Count,
};

enum class ERHIAttachmentLoadOperation {
  Load,
  Clear,
  DontCare,
  Count,
};

enum class ERHIAttachmentStoreOperation {
  Store,
  DontCare,
  Count,
};

enum class ERHIImageLayout {
  Undefined,
  General,
  ColorAttachment,
  DepthStencilAttachment,
  TransferSrc,
  TransferDst,
  ShaderReadOnly,
  PresentSrc,
  Count,
};

enum class ERHIPolygonMode { Fill, Line, Point, Count };

enum class ERHICullMode {
  None,
  Front,
  Back,
  FrontAndBack,
  Count,
};

enum class ERHIFrontFace {
  Clockwise,
  CounterClockwise,
  Count,
};

enum class ERHICompareOp {
  Less,
  LessOrEqual,
  Greater,
  GreaterOrEqual,
  Equal,
  NotEqual,
  Never,
  Always,
  Count,
};

enum class ERHIShaderStage {
  Vertex = 1,
  Fragment = 1 << 1,
  Compute = 1 << 2,
  MAX = 0x7FFFFFFF,
};
ENABLE_BITMASK_OPERATORS(ERHIShaderStage)
constexpr ERHIShaderStage KITA_RHI_ALL_GRAPHICS_SHADER_STAGES = ERHIShaderStage::Fragment | ERHIShaderStage::Vertex;

enum class ERHIDescriptorType {
  Sampler,
  UniformBuffer,
  SampledImage,
  StorageImage,
  Count,
};

enum ERHIAccessFlagBits {
  AFB_None = 0,
  AFB_ColorAttachmentRead = 1,
  AFB_ColorAttachmentWrite = 1 << 1,
  AFB_DepthStencilAttachmentRead = 1 << 2,
  AFB_DepthStencilAttachmentWrite = 1 << 3,
  AFB_TransferRead = 1 << 4,
  AFB_TransferWrite = 1 << 5,
  AFB_ShaderRead = 1 << 6,
  AFB_ShaderWrite = 1 << 7,
  AFB_Max = 0x7FFFFFFF,
};

using ERHIAccessFlags = int32_t;

enum ERHIPipelineStageFlagBits {
  PSFB_None = 0,
  PSFB_TopOfPipe = 1,
  PSFB_ColorAttachmentOutput = 1 << 1,
  PSFB_BottomOfPipe = 1 << 2,
  PSFB_DepthStencilAttachment = 1 << 3,
  PSFB_Transfer = 1 << 4,
  PSFB_FragmentShader = 1 << 5,
  PSFB_ComputeShader = 1 << 6,
  PSFB_Max = 0x7FFFFFFF,
};

using ERHIPipelineStageFlags = int32_t;

enum class ERHIVertexInputRate {
  Vertex,
  Instance,
  Count,
};

enum class ERHIImageUsage {
  None = 0,
  TransferSrc = 0b0000001,  // VK_IMAGE_USAGE_TRANSFER_SRC_BIT
  TransferDst = 0b0000010,  // VK_IMAGE_USAGE_TRANSFER_DST_BIT
  RenderTarget = 0b0000100, // VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
  DepthStencil = 0b0001000, // VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
  ShaderRead = 0b0010000,   // VK_IMAGE_USAGE_SAMPLED_BIT
  Transient = 0b0100000,    // VK_IMAGE_USAGE_STORAGE_BIT
  SwapChain = 0b1000000,    // For swapchain use
  Storage = 0b10000000,     // VK_IMAGE_USAGE_STORAGE_BIT
  Max,                      // VK_IMAGE_USAGE_MAX_ENUM
};
ENABLE_BITMASK_OPERATORS(ERHIImageUsage)

enum class ERHIImageDimension {
  D1, // 一维图像
  D2, // 二维图像
  D3, // 三维图像
  Cube,
  Array1D,
  Array2D,
  ArrayCube,
  Count,
};

enum class ERHIFilterMode {
  Linear,
  Nearest,
  Count,
};

enum class ERHISamplerAddressMode {
  Repeat,
  MirroredRepeat,
  ClampToEdge,
  Count,
};

enum class ERHIMipmapMode : uint8_t {
  Nearest = 0, // VK_SAMPLER_MIPMAP_MODE_NEAREST
  Linear = 1   // VK_SAMPLER_MIPMAP_MODE_LINEAR
};

enum class ERHIBorderColor : uint8_t {
  FloatTransparentBlack = 0, // VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK
  IntTransparentBlack = 1,   // VK_BORDER_COLOR_INT_TRANSPARENT_BLACK
  FloatOpaqueBlack = 2,      // VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK
  IntOpaqueBlack = 3,        // VK_BORDER_COLOR_INT_OPAQUE_BLACK
  FloatOpaqueWhite = 4,      // VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE
  IntOpaqueWhite = 5         // VK_BORDER_COLOR_INT_OPAQUE_WHITE
};

enum class ERHIBlendFactor {
  Zero,
  One,
  SrcAlpha,
  OneMinusSrcAlpha,
  Count,
};

enum class ERHIBlendOp {
  Add,
  Count,
};

// vulkan当前创建交换链的时候用了这个东西
enum ERHISurfaceTransformBits {
  STFB_Identity = 0x00000001,
  STFB_Rotate90 = 0x00000002,
  STFB_Rotate180 = 0x00000004,
  STFB_Rotate270 = 0x00000008,
  STFB_HorizontalMirror = 0x00000010,
  STFB_HorizontalMirrorRotate90 = 0x00000020,
  STFB_HorizontalMirrorRotate180 = 0x00000040,
  STFB_HorizontalMirrorRotate270 = 0x00000080,
  STFB_Inherit = 0x00000100,
  STFB_Max = 0x7FFFFFFF
};
typedef UInt32 ERHISurfaceTransform;

enum class ERHIPipelineBindPoint { Graphics, Compute, Count };

enum class ERHIPrimitiveTopology {
  TriangleList,
  Count,
};

enum ERHIColorComponentBits {
  ECCB_R = 1,
  ECCB_G = 1 << 1,
  ECCB_B = 1 << 2,
  ECCB_A = 1 << 3,
};
using ERHIColorComponent = UInt32;

enum class ERHILogicOp { Copy, Count };