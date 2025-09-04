import Annotation;
import Common;

[Dynamic, HideInEditor]
uniform ParameterBlock<float4x4> gModel;

[Shared("Camera"), HideInEditor]
uniform ParameterBlock<Camera> gCamera;

struct Params {
  TextureCube SkyboxTexture;
  SamplerState SkyboxSampler;
}

uniform ParameterBlock<Params> gSkybox;

struct VO {
  float3 Position : SV_Position;
  float2 UV0;
}

[shader("vertex")]
VO VertexMain(Vertex In) {
  VO Out = {};
  Out.Position = In.Position;
  Out.UV0 = In.UV0;
  return Out;
}

struct FO {
  float4 Color : SV_Target;
}

/// 天空盒不适用背面剔除, 而使用正面剔除
[shader("fragment"), CullMode(false)]
FO FragmentMain(VO In) {
  FO Out;
  Out.Color = float4(1, 0, 1, 1);
  return Out;
}
