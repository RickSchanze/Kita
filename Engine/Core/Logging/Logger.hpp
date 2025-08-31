#pragma once

#include "Core/Macros.h"
#include "Core/String/String.h"
#include "Core/String/StringTraits.h"
#include "Core/String/StringView.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "Core/Event/Callback.h"
#include "Core/Memory/SharedPtr.h"
#include "Core/Trace.h" // for Error and Fatal log stack trace

#include "fmt/ostream.h"
#include "spdlog/spdlog.h"
enum class ELogLevel { Info, Warn, Error, Critical };

struct Log {
  StringView Tag;
  String Message;
  ELogLevel Level;
};

class Logger {
public:
  using CallbackHandle = Callback<void, const Log&>::Handle;
  Callback<void, const Log&> Evt_Log;

  Logger();

  spdlog::logger& GetLogger() { return mLogger; }

  template <typename... Args> void Info(StringView Tag, fmt::format_string<Args...> FormatString, Args&&... InArgs) {
    Log NewLog{};
    NewLog.Message = fmt::format(FormatString, std::forward<Args>(InArgs)...);
    NewLog.Tag = Tag;
    NewLog.Level = ELogLevel::Info;
    mLogger.info("[{}] {}", Tag, NewLog.Message);
    Evt_Log.Invoke(NewLog);
  }

  template <typename... Args> void Warn(StringView Tag, fmt::format_string<Args...> FormatString, Args&&... InArgs) {
    Log NewLog{};
    NewLog.Message = fmt::format(FormatString, std::forward<Args>(InArgs)...);
    NewLog.Tag = Tag;
    NewLog.Level = ELogLevel::Warn;
    mLogger.warn("[{}] {}", Tag, NewLog.Message);
    Evt_Log.Invoke(NewLog);
  }

  template <typename... Args> void Error(StringView Tag, fmt::format_string<Args...> FormatString, Args&&... InArgs) {
    Log NewLog{};
    NewLog.Message = fmt::format(FormatString, std::forward<Args>(InArgs)...);
    NewLog.Tag = Tag;
    NewLog.Level = ELogLevel::Error;
    mLogger.error("[{}] {}", Tag, NewLog.Message);
    Evt_Log.Invoke(NewLog);
  }

  template <typename... Args> void Critical(StringView Tag, fmt::format_string<Args...> FormatString, Args&&... InArgs) {
    Log NewLog{};
    NewLog.Message = fmt::format(FormatString, std::forward<Args>(InArgs)...);
    NewLog.Tag = Tag;
    NewLog.Level = ELogLevel::Critical;
    mLogger.critical("[{}] {}", Tag, NewLog.Message);
    Evt_Log.Invoke(NewLog);
    DEBUG_BREAK();
    std::abort();
  }

private:
  spdlog::logger mLogger;
};

inline Logger gLogger = {};

inline std::ostream& operator<<(std::ostream& OS, const String& Str) {
  OS << Str.GetStdString();
  return OS;
}
template <> struct fmt::formatter<String> : ostream_formatter {};

inline std::ostream& operator<<(std::ostream& OS, const StringView& Str) {
  OS << Str.GetStdStringView();
  return OS;
}
template <> struct fmt::formatter<StringView> : ostream_formatter {};

template <Traits::IToString T> std::ostream& operator<<(std::ostream& OS, const T& Str) {
  OS << Str.ToString();
  return OS;
}

template <Traits::IsEnum T> std::ostream& operator<<(std::ostream& OS, const T& Str) {
  OS << static_cast<int>(Str);
  return OS;
}

template <Traits::IToString T> struct fmt::formatter<T> : ostream_formatter {};
template <Traits::IsEnum T> struct fmt::formatter<T> : ostream_formatter {};

template <typename T> void* Ptr(T* InPtr) { return InPtr; }
template <typename T> void* Ptr(const T* InPtr) { return InPtr; }
template <typename T> void* Ptr(SharedPtr<T>& InPtr) { return InPtr.Get(); }
template <typename T> void* Ptr(const SharedPtr<T>& InPtr) { return InPtr.Get(); }

template <typename... Args> String Format(fmt::format_string<Args...> FormatString, Args&&... InArgs) { return String(fmt::format(FormatString, std::forward<Args>(InArgs)...)); }
