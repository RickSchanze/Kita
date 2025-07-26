//
// Created by Tencent_GO on 25-7-24.
//

#include "StringView.h"

#include "String.h"

StringView::StringView(const String& Str) : mStr(Str.GetStdStringView()) {}