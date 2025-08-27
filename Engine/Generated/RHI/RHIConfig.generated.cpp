// Auto-generated source file
#include "RHI/RHIConfig.h"

const Type* RHIConfig::GetStaticType() { return TypeOf<RHIConfig>(); }
const Type* RHIConfig::GetType() { return TypeOf<RHIConfig>(); }
void RHIConfig::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("GraphicsBackend", GraphicsBackend); 
Archive.WriteType("PresentMode", PresentMode); 
Archive.WriteType("SurfaceWindowType", SurfaceWindowType); 
Archive.WriteType("EnableVulkanValidationLayer", EnableVulkanValidationLayer); 
Archive.WriteType("DesiredSwapchainFormat", DesiredSwapchainFormat); 
Archive.WriteType("DesiredSwapchainColorSpace", DesiredSwapchainColorSpace); 
Archive.WriteType("DefaultWindowSize", DefaultWindowSize); 
} 
void RHIConfig::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("GraphicsBackend", GraphicsBackend); 
Archive.ReadType("PresentMode", PresentMode); 
Archive.ReadType("SurfaceWindowType", SurfaceWindowType); 
Archive.ReadType("EnableVulkanValidationLayer", EnableVulkanValidationLayer); 
Archive.ReadType("DesiredSwapchainFormat", DesiredSwapchainFormat); 
Archive.ReadType("DesiredSwapchainColorSpace", DesiredSwapchainColorSpace); 
Archive.ReadType("DefaultWindowSize", DefaultWindowSize); 
} 
