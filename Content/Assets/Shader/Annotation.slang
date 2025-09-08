module Annotation;

[__AttributeUsage(_AttributeTargets.Function)]
public struct CullModeAttribute {
  bool UseBackface;
}

[__AttributeUsage(_AttributeTargets.Var)]
public struct SharedAttribute {
  string Name;
}

[__AttributeUsage(_AttributeTargets.Var)]
public struct DynamicAttribute {}

[__AttributeUsage(_AttributeTargets.Var)]
public struct HideInEditorAttribute {}

