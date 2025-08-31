//
// Created by kita on 25-8-31.
//

#include "StringUtils.h"
Array<StringView> StringUtils::Split(const StringView Str, const char Delimiter) {
  Array<StringView> Result;
  auto StdStr = Str.GetStdStringView();
  SizeType Start = 0;
  SizeType Pos = 0;

  while (Pos != std::string_view::npos) {
    Pos = StdStr.find(Delimiter, Start);
    if (Pos == std::string_view::npos) {
      // 最后一个片段
      Result.Add(StringView(StdStr.substr(Start)));
      break;
    } else {
      Result.Add(StringView(StdStr.substr(Start, Pos - Start)));
      Start = Pos + 1;
    }
  }

  return Result;
}