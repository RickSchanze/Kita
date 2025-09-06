//
// Created by Tencent_GO on 25-7-24.
//

#include "String.h"

#include "StringView.h"

String::String(const StringView& Str) : mStr(Str.GetStdStringView()) {}

bool String::EndsWith(const StringView& Suffix) const { return mStr.ends_with(Suffix.GetStdStringView()); }

String& String::Replace(const char OldChar, const char NewChar) {
  for (char& c : mStr) {
    if (c == OldChar) {
      c = NewChar;
    }
  }
  return *this;
}

String& String::Replace(const StringView& OldStr, const StringView& NewStr) {
  const auto OldStdStr = OldStr.GetStdStringView(); // 假设StringView有GetStdString方法
  const auto NewStdStr = NewStr.GetStdStringView();

  if (OldStdStr.empty()) {
    return *this; // 不能替换空字符串
  }

  std::size_t Pos = 0;
  while ((Pos = mStr.find(OldStdStr, Pos)) != std::string::npos) {
    mStr.replace(Pos, OldStdStr.length(), NewStdStr);
    Pos += NewStdStr.length();
  }
  return *this;
}

String operator+(const String& Str1, const StringView& Str2) {
  String Str3(Str1);
  return Str3 += String(Str2);
}

String operator+(const String& Str1, const char* Str2) {
  String Str3(Str1);
  return Str3 += Str2;
}

std::istream& operator>>(std::istream& IS, String& Str)  {
  IS >> Str.mStr;
  return IS;
}