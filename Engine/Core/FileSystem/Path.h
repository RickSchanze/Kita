#pragma once

#include "Core/String/String.h"

struct Path {
public:
  /// 构造一个路径
  /// @param Path 输入的路径
  /// @param NeedNormalize 是否正则化路径
  Path(StringView Path, bool NeedNormalize = true);

  /// 正则化一个路径 例如: C:/a/../b 会变成 C:/b
  /// @note 修改是Inplace的
  /// @return 正则化后的路径
  void Normalize();

  /// C:/a/../b -> C:/
  /// @return 返回一个正则化之后的路径
  [[nodiscard]] Path Normalized() const;

  [[nodiscard]] std::string GetStdString() const { return mPath.GetStdString(); }

  [[nodiscard]] String ToString() const { return mPath; }

  static inline char Separator = '/';

  static bool IsExists(StringView Path);
  bool IsExists() const;

  static bool IsDirectory(StringView Path);
  bool IsDirectory() const;

  // 也可以提供直接 Combine 两个字符串的版本：
  static String Combine(StringView Base, StringView Relative, bool NeedNormalize = true);
  static bool CreateDirectory(StringView Path, bool Recursive = true) ;


private:
  String mPath;
};
