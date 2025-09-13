// Auto-generated source file
#include "Assets/Shader.h"

void Z_Reflection_Register_Func_Enum_EShaderLanguage() {TypeBuilder Builder{};Builder.CreateType<EShaderLanguage>("EShaderLanguage");
Builder.AddField("Slang", EShaderLanguage::Slang);
Builder.AddField("Spirv", EShaderLanguage::Spirv);
Builder.AddField("GLSL", EShaderLanguage::GLSL);
Builder.AddField("HLSL", EShaderLanguage::HLSL);
Builder.AddField("Count", EShaderLanguage::Count);
Builder.Register();
}const Type* ShaderMeta::GetStaticType() { return TypeOf<ShaderMeta>(); }
const Type* ShaderMeta::GetType() { return TypeOf<ShaderMeta>(); }
void ShaderMeta::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
Archive.WriteType("Id", Id); 
Archive.WriteType("Path", Path); 
Archive.WriteType("ObjectHandle", ObjectHandle); 
} 
void ShaderMeta::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
Archive.ReadType("Id", Id); 
Archive.ReadType("Path", Path); 
Archive.ReadType("ObjectHandle", ObjectHandle); 
} 
const Type* Shader::GetStaticType() { return TypeOf<Shader>(); }
const Type* Shader::GetType() { return TypeOf<Shader>(); }
void Shader::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
} 
void Shader::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
} 
