import Annotation;
import Common;

[Dynamic, HideInEditor]
uniform ParameterBlock<float4x4> Model;

[Shared("Camera"), HideInEditor]
uniform ParameterBlock<Camera> Camera;

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

[shader("fragment")]
FO FragmentMain(VO In) {
  FO Out;
  Out.Color = float4(1, 0, 1, 1);
  return Out;
}
