#pragma once

#include "Core/Macros.h"
#include "Core/String/String.h"
#include "Core/String/StringTraits.h"
#include "Core/String/StringView.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

#include <fmt/ostream.h>

class Logger {
public:
  Logger();

  spdlog::logger& GetLogger() { return mLogger; }

private:
  spdlog::logger mLogger;
};

inline Logger gLogger = {};

#define LOG_INFO(...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::warn, __VA_ARGS__)

#ifdef KITA_DEBUG
#define LOG_ERROR(...) \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__); \
  DEBUG_BREAK()

#define LOG_CRITICAL(...) \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__); \
  DEBUG_BREAK()

#else
#define LOG_ERROR(...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__)
#endif

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

template <Traits::IStringable T> std::ostream& operator<<(std::ostream& OS, const T& Str) {
  OS << Str.ToString();
  return OS;
}

template <Traits::IStringable T> struct fmt::formatter<T> : ostream_formatter {};
