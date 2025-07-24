//
// Created by Tencent_GO on 25-7-24.
//

#include "String.h"

#include "StringView.h"

String::String(const StringView& Str) : mStr(Str.GetStdStringView()) {}