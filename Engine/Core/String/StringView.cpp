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