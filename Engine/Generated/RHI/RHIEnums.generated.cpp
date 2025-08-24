// Auto-generated source file
#include "RHI/RHIEnums.h"

void Z_Reflection_Register_Func_Enum_ERHIFormat() {TypeBuilder Builder{};Builder.CreateType<ERHIFormat>("ERHIFormat");
Builder.AddField("R32G32B32_Float", ERHIFormat::R32G32B32_Float);
Builder.AddField("R32G32B32A32_Float", ERHIFormat::R32G32B32A32_Float);
Builder.AddField("R8G8B8_UNorm", ERHIFormat::R8G8B8_UNorm);
Builder.AddField("R16G16B16A16_UNorm", ERHIFormat::R16G16B16A16_UNorm);
Builder.AddField("R32G32_UInt", ERHIFormat::R32G32_UInt);
Builder.AddField("R8G8B8A8_UNorm", ERHIFormat::R8G8B8A8_UNorm);
Builder.AddField("R8G8B8A8_SNorm", ERHIFormat::R8G8B8A8_SNorm);
Builder.AddField("R8G8B8A8_UInt", ERHIFormat::R8G8B8A8_UInt);
Builder.AddField("R8G8B8A8_SInt", ERHIFormat::R8G8B8A8_SInt);
Builder.AddField("R8G8B8A8_SRGB", ERHIFormat::R8G8B8A8_SRGB);
Builder.AddField("D32_Float_S8X24_UInt", ERHIFormat::D32_Float_S8X24_UInt);
Builder.AddField("D32_Float", ERHIFormat::D32_Float);
Builder.AddField("D24_UNorm_S8_UInt", ERHIFormat::D24_UNorm_S8_UInt);
Builder.AddField("D16_UNorm", ERHIFormat::D16_UNorm);
Builder.AddField("B8G8R8A8_SRGB", ERHIFormat::B8G8R8A8_SRGB);
Builder.AddField("B8G8R8A8_UNorm", ERHIFormat::B8G8R8A8_UNorm);
Builder.AddField("R16G16B16A16_Float", ERHIFormat::R16G16B16A16_Float);
Builder.AddField("A2B10G10R10_UNorm", ERHIFormat::A2B10G10R10_UNorm);
Builder.AddField("R32G32_Float", ERHIFormat::R32G32_Float);
Builder.AddField("R32_Float", ERHIFormat::R32_Float);
Builder.AddField("R8_UNorm", ERHIFormat::R8_UNorm);
Builder.AddField("R8_SRGB", ERHIFormat::R8_SRGB);
Builder.AddField("Count", ERHIFormat::Count);
Builder.Register();
}void Z_Reflection_Register_Func_Enum_ERHIPresentMode() {TypeBuilder Builder{};Builder.CreateType<ERHIPresentMode>("ERHIPresentMode");
Builder.AddField("VSync", ERHIPresentMode::VSync);
Builder.AddField("Immediate", ERHIPresentMode::Immediate);
Builder.AddField("TripleBuffer", ERHIPresentMode::TripleBuffer);
Builder.AddField("Count", ERHIPresentMode::Count);
Builder.Register();
}