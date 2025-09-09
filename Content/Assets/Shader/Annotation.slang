module Annotation;

[__AttributeUsage(_AttributeTargets.Function)]
public struct CullModeAttribute {
  bool UseBackface;
}

/// 用于标注ParameterBlock 表示此Set是否被共享
[__AttributeUsage(_AttributeTargets.Var)]
public struct SharedAttribute {
  string Name;
}

[__AttributeUsage(_AttributeTargets.Var)]
public struct LabelAttribute {
  string Name;
}

/// 用于标注ParameterBlock 表示此Set为DynamicUniformBuffer 并且要求ParameterBlock里面的只有Buffer 没有Resource
[__AttributeUsage(_AttributeTargets.Var)]
public struct DynamicAttribute {}

[__AttributeUsage(_AttributeTargets.Var)]
public struct HideInEditorAttribute {}

