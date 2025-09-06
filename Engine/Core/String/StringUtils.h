#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Optional.h"

#include <charconv>

class StringUtils {
public:
  static Array<StringView> Split(StringView Str, char Delimiter);

  template <typename T> static Optional<T> ToNumber(const StringView Str) {
    std::string_view StdView = Str.GetStdStringView();

    // 去除前后空白字符
    while (!StdView.empty() && std::isspace(StdView.front())) {
      StdView.remove_prefix(1);
    }
    while (!StdView.empty() && std::isspace(StdView.back())) {
      StdView.remove_suffix(1);
    }

    if (StdView.empty()) {
      return Optional<T>{}; // 返回空的Optional
    }

    T Value{};

    if constexpr (std::is_floating_point_v<T>) {
      // 浮点数处理
      if (auto result = std::from_chars(StdView.data(), StdView.data() + StdView.size(), Value); result.ec == std::errc{} && result.ptr == StdView.data() + StdView.size()) {
        return Optional<T>{Value};
      }
    } else if constexpr (std::is_integral_v<T>) {
      // 整数处理
      int Base = 10;

      // 检查是否是十六进制
      if (StdView.size() > 2 && StdView.substr(0, 2) == "0x") {
        Base = 16;
        StdView.remove_prefix(2);
      } else if (StdView.size() > 2 && StdView.substr(0, 2) == "0X") {
        Base = 16;
        StdView.remove_prefix(2);
      } else if (StdView.size() > 1 && StdView[0] == '0' && StdView.size() > 1) {
        // 检查是否是八进制（可选功能）
        bool IsOctal = true;
        for (char C : StdView.substr(1)) {
          if (C < '0' || C > '7') {
            IsOctal = false;
            break;
          }
        }
        if (IsOctal) {
          Base = 8;
        }
      }

      if (auto Result = std::from_chars(StdView.data(), StdView.data() + StdView.size(), Value, Base); Result.ec == std::errc{} && Result.ptr == StdView.data() + StdView.size()) {
        return Optional<T>{Value};
      }
    } else {
      // 不支持的类型
      static_assert(std::is_arithmetic_v<T>, "ToNumber only supports arithmetic types");
    }

    return Optional<T>{}; // 转换失败，返回空Optional
  }
};