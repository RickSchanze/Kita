//
// Created by Tencent_GO on 25-7-24.
//

#include "StringView.h"

#include "String.h"

StringView::StringView(const String& Str) : mStr(Str.GetStdString()) {}

String StringView::ToString() const { return std::string{mStr}; }

SizeType StringView::IndexOf(const char Target, const SizeType Start) const { return mStr.find_first_of(Target, Start); }

SizeType StringView::LastIndexOf(const char Target, const SizeType Start) const { return mStr.find_last_of(Target, Start); }

SizeType StringView::IndexOf(const StringView Target, const SizeType Start) const { return mStr.find_first_of(Target.GetStdStringView(), Start); }

SizeType StringView::LastIndexOf(const StringView Target, const SizeType Start) const { return mStr.find_last_of(Target.GetStdStringView(), Start); }

StringView StringView::SubStr(const SizeType Start, const SizeType Length) const { return StringView{mStr.substr(Start, Length)}; }

static bool ContainsChar(StringView Chars, const char Char) {
  for (const auto c : Chars) {
    if (c == Char) return true;
  }
  return false;
}

StringView StringView::TrimLeft(const char Char) const {
  size_t Start = 0;
  while (Start < mStr.size() && mStr[Start] == Char) {
    ++Start;
  }
  return {mStr.data() + Start, mStr.size() - Start};
}

StringView StringView::TrimLeft(const StringView Chars) const {
  size_t Start = 0;
  while (Start < mStr.size() && ContainsChar(Chars, mStr[Start])) {
    ++Start;
  }
  return {mStr.data() + Start, mStr.size() - Start};
}

StringView StringView::TrimRight(const char Char) const {
  size_t End = mStr.size();
  while (End > 0 && mStr[End - 1] == Char) {
    --End;
  }
  return {mStr.data(), End};
}

StringView StringView::TrimRight(const StringView Chars) const {
  size_t End = mStr.size();
  while (End > 0 && ContainsChar(Chars, mStr[End - 1])) {
    --End;
  }
  return {mStr.data(), End};
}

StringView StringView::Trim(const char Char) const {
  return TrimLeft(Char).TrimRight(Char);
}

StringView StringView::Trim(const StringView Chars) const {
  return TrimLeft(Chars).TrimRight(Chars);
}