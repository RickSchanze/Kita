//
// Created by kita on 25-8-6.
//

#include "Path.h"

#include <filesystem>

using namespace std::filesystem;

Path::Path(const StringView Path, bool Normalize) : mPath(Path) {
    if (Normalize) {
      Normalize();
    }
}

void Path::Normalize() {
  // TODO: Inplace copy string
  path Temp = mPath.GetStdString();
  Temp = Temp.lexically_normal();
  mPath = Temp.string();
}

Path Path::Normalized() const {
  const path Temp = mPath.GetStdString();
  const auto TempStr = Temp.lexically_normal().string();
  return {StringView(TempStr.data(), TempStr.size())};
}