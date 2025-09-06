//
// Created by kita on 25-9-5.
//

#include "Shader.h"

#include "AssetsConfig.h"
#include "Core/FileSystem/File.h"
#include "Core/FileSystem/InputFileStream.h"
#include "Core/FileSystem/OutputFileStream.h"
#include "Core/FileSystem/Path.h"
#include "Core/String/StringUtils.h"
#include "Project/Project.h"

#include "Core/Config/ConfigManager.h"
#include "slang-com-ptr.h"

class SlangShaderTranslater {
public:
  SlangShaderTranslater() {
    slang::createGlobalSession(mGlobalSession.writeRef());
    CreateSession();
  }

  Slang::ComPtr<slang::ISession> GetSession() { return mCompileSession; }

private:
  void CreateSession() {
    auto& Config = ConfigManager::GetConfigRef<AssetsConfig>();
    auto SearchPaths = Config.GetShaderSearchPaths();
    Array<const char*> SearchPathsArray;
    for (auto& SearchPath : SearchPaths) {
      SearchPathsArray.Add(SearchPath.Data());
    }
    slang::SessionDesc Desc{};
    slang::TargetDesc TargetDesc{};
    TargetDesc.format = SLANG_SPIRV;
    TargetDesc.profile = mGlobalSession->findProfile("glsl_460");
    Desc.searchPaths = SearchPathsArray.Data();
    Desc.searchPathCount = SearchPathsArray.Count();
    Desc.targets = &TargetDesc;
    Desc.targetCount = 1;
    mGlobalSession->createSession(Desc, mCompileSession.writeRef());
  }

  Slang::ComPtr<slang::IGlobalSession> mGlobalSession;
  Slang::ComPtr<slang::ISession> mCompileSession;
};

static SlangShaderTranslater& GetTranslater() {
  static SlangShaderTranslater Translater;
  return Translater;
}

void Shader::Load() {
  if (mPath.Empty() || !mPath.EndsWith(".slang")) {
    gLogger.Error(Logcat::Asset, "无效的Shader路径.");
    return;
  }
  ReadCache();
  if (NeedReTranslate()) {
    mLoaded = Translate();
  } else {
    mLoaded = ReadBinary();
  }
  if (mLoaded) {
    gLogger.Info(Logcat::Asset, "Shader '{}' 加载成功.", mPath);
  } else {
    gLogger.Error(Logcat::Asset, "Shader '{}' 加载失败.", mPath);
  }
}

void Shader::Unload() {
  mShaderData.Data.Clear();
  mLoaded = false;
}

void Shader::ApplyMeta(const AssetMeta& Meta) {
  const auto& MyShaderMeta = static_cast<const ShaderMeta&>(Meta);
  mPath = MyShaderMeta.Path;
  // Apply时Handle已经处理
}

constexpr auto ShaderCacheFolder = "Shader/Cache";

String Shader::GetBinaryPath() {
  const auto ThisShaderBinaryFileName = mPath.Replace(".slang", ".shaderbin");
  return Path::Combine(Path::Combine(Project::GetIntermediatePath(), ShaderCacheFolder), ThisShaderBinaryFileName);
}

bool Shader::NeedReTranslate() {
  if (sCache.Contains(mPath)) {
    const ShaderCache& OldCache = sCache[mPath];
    if (const auto NewTime = File::GetLastModifiedTime(mPath); (!NewTime || OldCache.LastTextModifiedTime != *NewTime)) {
      return true;
    }
    if (const auto ThisShaderBinaryPath = GetBinaryPath(); Path::IsExists(ThisShaderBinaryPath)) {
      if (const auto BinaryTime = File::GetLastModifiedTime(ThisShaderBinaryPath); (!BinaryTime || OldCache.LastBinaryModifiedTime != *BinaryTime)) {
        return true;
      }
      return false;
    } else {
      return true;
    }
  }
  return true;
}

#define DIAG()                                                                                                                        \
  if (Diagnostics) {                                                                                                                  \
    gLogger.Error(Logcat::Asset, "读取Shader '{}' 时发生错误: {}", mPath, static_cast<const char*>(Diagnostics->getBufferPointer())); \
    return false;                                                                                                                     \
  }

static bool TranslateCompute(Slang::ComPtr<slang::IEntryPoint> ComputeEntry, ShaderBinaryData& OutBinaryData) { return false; }

static bool TranslateGraphics(Slang::ComPtr<slang::IEntryPoint> VertexEntry, Slang::ComPtr<slang::IEntryPoint> FragmentEntry, ShaderBinaryData& OutBinaryData) {

}

bool Shader::Translate() {
  using namespace slang;
  Slang::ComPtr<IBlob> Diagnostics;
  IModule* Module = GetTranslater().GetSession()->loadModule(mPath.Data(), Diagnostics.writeRef());
  DIAG();
  Slang::ComPtr<IEntryPoint> ComputeEntryPoint;
  Module->findEntryPointByName("ComputeMain", ComputeEntryPoint.writeRef());
  if (ComputeEntryPoint) {
    DIAG();
    return TranslateCompute(ComputeEntryPoint, mShaderData);
  }
  Slang::ComPtr<IEntryPoint> VertexEntryPoint;
  Module->findEntryPointByName("VertexMain", VertexEntryPoint.writeRef());
  DIAG();
  Slang::ComPtr<IEntryPoint> FragmentEntryPoint;
  Module->findEntryPointByName("FragmentMain", FragmentEntryPoint.writeRef());
  DIAG();
  return TranslateGraphics(VertexEntryPoint, FragmentEntryPoint, mShaderData);
}

bool Shader::ReadBinary() {
  InputFileStream InputFile(GetBinaryPath(), std::ios::in | std::ios::binary);
  const auto FileSize = InputFile.GetFileSize();
  if (!FileSize) {
    return false;
  }
  mShaderData.Data.Resize(*FileSize);
  UInt64 BytesRead = 0;
  InputFile.ReadBytes(mShaderData.Data.Data(), mShaderData.Data.Count(), BytesRead);
  if (BytesRead < 12) {
    return false;
  }
  return true;
}

constexpr auto ShaderCacheFile = "Shader/Cache.csv";

void Shader::ReadCache() {
  if (sCacheRead) {
    return;
  }
  if (auto IntermediateShaderCachePath = Path::Combine(Project::GetIntermediatePath(), ShaderCacheFile); Path::IsExists(IntermediateShaderCachePath)) {
    if (Path::IsDirectory(IntermediateShaderCachePath)) {
      gLogger.Error(Logcat::Asset, "ShaderCachePath '{}' 不能是一个目录.", IntermediateShaderCachePath);
      return;
    }
    InputFileStream InputFile(IntermediateShaderCachePath);
    Int32 LineNum = 9;
    while (true) {
      LineNum++;
      if (auto LineResult = InputFile.ReadLine()) {
        auto& Line = *LineResult;
        Array<StringView> Words = StringUtils::Split(Line, ',');
        if (Words.Count() != 3) {
          gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'第{}行错误.", IntermediateShaderCachePath, LineNum);
          continue;
        } else {
          ::ShaderCache ShaderCache{};
          auto LastBinaryModifiedTime = StringUtils::ToNumber<UInt64>(Words[1]);
          if (!LastBinaryModifiedTime) {
            gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'第{}行错误.", IntermediateShaderCachePath, LineNum);
            continue;
          }
          ShaderCache.LastBinaryModifiedTime = *LastBinaryModifiedTime;
          const auto LastTextModifiedTime = StringUtils::ToNumber<UInt64>(Words[2]);
          if (!LastTextModifiedTime) {
            gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'第{}行错误.", IntermediateShaderCachePath, LineNum);
            continue;
          }
          ShaderCache.LastTextModifiedTime = *LastTextModifiedTime;
          if (!Words[0].Empty()) {
            sCache[Words[0].ToString()] = ShaderCache;
          } else {
            gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'第{}行错误.", IntermediateShaderCachePath, LineNum);
          }
        }
      } else {
        if (LineResult.Error() != EFileSystemError::EndOfFile) {
          gLogger.Error(Logcat::Asset, "读取ShaderCache文件'{}'时发生错误: {}", IntermediateShaderCachePath, LineResult.Error());
        }
      }
    }
  }
  sCacheRead = true;
}

void Shader::WriteCache() {
  const auto IntermediateShaderCachePath = Path::Combine(Project::GetIntermediatePath(), ShaderCacheFile);
  OutputFileStream OutputFile(IntermediateShaderCachePath);
  for (auto& [Path, Cache] : sCache) {
    OutputFile.WriteLine(Path, ",", Cache.LastBinaryModifiedTime, ",", Cache.LastTextModifiedTime);
  }
}