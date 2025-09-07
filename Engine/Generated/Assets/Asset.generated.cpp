// Auto-generated source file
#include "Assets/Asset.h"

void Z_Reflection_Register_Func_Enum_EAssetType() {TypeBuilder Builder{};Builder.CreateType<EAssetType>("EAssetType");
Builder.AddField("Shader", EAssetType::Shader);
Builder.AddField("Mesh", EAssetType::Mesh);
Builder.AddField("Texture", EAssetType::Texture);
Builder.AddField("Material", EAssetType::Material);
Builder.AddField("Scene", EAssetType::Scene);
Builder.AddField("Animation", EAssetType::Animation);
Builder.AddField("Audio", EAssetType::Audio);
Builder.AddField("Font", EAssetType::Font);
Builder.AddField("Prefab", EAssetType::Prefab);
Builder.AddField("Count", EAssetType::Count);
Builder.Register();
}const Type* Asset::GetStaticType() { return TypeOf<Asset>(); }
const Type* Asset::GetType() { return TypeOf<Asset>(); }
void Asset::WriteArchive(OutputArchive& Archive) const { 
Super::WriteArchive(Archive); 
} 
void Asset::ReadArchive(InputArchive& Archive) { 
Super::ReadArchive(Archive); 
} 
