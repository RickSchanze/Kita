//
// Created by Tencent_GO on 25-7-24.
//

#include "String.h"

#include "StringView.h"

String::String(const StringView& Str) : mStr(Str.GetStdStringView()) {}

String operator+(const String& Str1, const StringView& Str2) {
  String Str3(Str1);
  return Str3 += String(Str2);
}

String operator+(const String& Str1, const char* Str2) {
  String Str3(Str1);
  return Str3 += Str2;
}