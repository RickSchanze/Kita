module Common;

/// 静态网格体的顶点结构定义
public struct Vertex {
  public float3 Position;
  public float3 Normal;
  public float2 UV0;
}

public struct Camera {
  public float4x4 Projection;
  public float4x4 View;
  /// MVP中的VP
  public float4x4 ViewProjection;
  /// 摄像机的其他参数, 例如NearPlane FarPlane等等
  public float4x4 Params;
}
