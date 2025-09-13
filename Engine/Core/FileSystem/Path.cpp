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

String Path::Combine(StringView Base, StringView Relative, bool NeedNormalize) {
  std::string result(Base.begin(), Base.end());

  if (!result.empty() && result.back() != Path::Separator)
    result.push_back(Path::Separator);

  std::string RelStr(Relative.begin(), Relative.end());
  if (!RelStr.empty() && RelStr.front() == Path::Separator)
    RelStr.erase(RelStr.begin());

  result += RelStr;
  const Path Combined(result, /*NeedNormalize*/ NeedNormalize);
  return Combined.GetStdString();
}

bool Path::CreateDirectory(StringView Path, bool Recursive) {
  namespace fs = std::filesystem;

  if (Path.Empty())
    return false;

  const fs::path FsPath(Path.GetStdStringView());

  std::error_code ER;
  bool OK = false;
  if (Recursive) {
    OK = fs::create_directories(FsPath, ER); // 创建所有父目录
  } else {
    OK = fs::create_directory(FsPath, ER); // 只创建一层
  }

  // 如果目录已经存在，fs::create_* 会返回 false，但是并不算错误
  return OK || fs::exists(FsPath);
}

StringView Path::GetExtension(const StringView Path) {
  const SizeType DotPos = Path.IndexOf('.');
  const SizeType SepPos = Path.LastIndexOf(Separator);
  if (DotPos == std::string::npos || (SepPos != std::string::npos && DotPos < SepPos)) {
    return {""};
  }

  // 跳过点号，从点后面开始截取
  return Path.SubStr(DotPos + 1);
}

StringView Path::GetFileName(StringView Path) {
  if (Path.Empty()) {
    return {};
  }

  // 查找最后一个路径分隔符（支持 Windows 和 Unix 风格）
  const SizeType LastSlash = Path.LastIndexOf('/');
  const SizeType LastBackslash = Path.LastIndexOf('\\');

  // 找到最后一个分隔符的位置
  SizeType LastSeparator = std::string::npos;
  if (LastSlash != std::string::npos && LastBackslash != std::string::npos) {
    LastSeparator = std::max(LastSlash, LastBackslash);
  } else if (LastSlash != std::string::npos) {
    LastSeparator = LastSlash;
  } else if (LastBackslash != std::string::npos) {
    LastSeparator = LastBackslash;
  }

  // 如果没有找到分隔符，整个路径就是文件名
  if (LastSeparator == std::string::npos) {
    return Path;
  }

  // 返回分隔符之后的部分
  return Path.SubStr(LastSeparator + 1);
}

StringView Path::GetFileNameWithoutExtension(StringView Path) {
  const StringView FileName = GetFileName(Path);

  if (FileName.Empty()) {
    return {};
  }

  // 查找最后一个点的位置
  const SizeType LastDot = FileName.LastIndexOf('.');

  // 如果没有找到点，或者点在开头（隐藏文件），返回整个文件名
  if (LastDot == std::string::npos || LastDot == 0) {
    return FileName;
  }

  // 返回点之前的部分
  return FileName.SubStr(0, LastDot);
}
