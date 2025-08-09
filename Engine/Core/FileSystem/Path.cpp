//
// Created by kita on 25-8-6.
//

#include "Path.h"

#include <filesystem>

using namespace std::filesystem;

Path::Path(const StringView Path, const bool NeedNormalize) : mPath(Path) {
  if (NeedNormalize) {
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

bool Path::IsExists(const StringView Path) { return exists(Path.Data()); }
bool Path::IsExists() const { return IsExists(mPath); }
bool Path::IsDirectory(const StringView Path) { return is_directory(Path.Data()); }
bool Path::IsDirectory() const { return IsDirectory(mPath); }