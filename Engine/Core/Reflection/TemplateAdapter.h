#pragma once

struct TemplateAdapter {};
/// 用法
template <typename T> class YourTemplate : public TemplateAdapter {
  /// 注册成员字段时会使用此类型进行反射而不是YourTemplate<T>
  /// 例如KPROPERTY() YourTemplate<int> m; m的DeclaringType被标记为int
  /// 具体示例可看ObjectPtr
  using ReflectionType = T;
};
