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
#define LOG_INFO_TAG(Tag, ...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::info, "[" Tag "] " __VA_ARGS__)

#define LOG_WARN(...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::warn, __VA_ARGS__)
#define LOG_WARN_TAG(Tag, ...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::warn, "[" Tag "] " __VA_ARGS__)

#ifdef KITA_DEBUG
#define LOG_ERROR(...) \
  String Trace_ = Trace::GenerateTraceString(0); \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__); \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, "{}", Trace_); \
  DEBUG_BREAK()

#define LOG_ERROR_TAG(Tag, ...) \
  String Trace_ = Trace::GenerateTraceString(0); \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, "[" Tag "] " __VA_ARGS__); \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, "{}", Trace_); \
  DEBUG_BREAK()

#define LOG_CRITICAL(...) \
  String Trace_ = Trace::GenerateTraceString(0); \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__); \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, "{}", Trace_); \
  DEBUG_BREAK()

#define LOG_CRITICAL_TAG(Tag, ...) \
  String Trace_ = Trace::GenerateTraceString(0); \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, "[" TAG "] " __VA_ARGS__); \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, "{}", Trace_); \
  DEBUG_BREAK()

#else
#define LOG_ERROR(...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__)
#define LOG_ERROR_TAG(Tag, ...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, "[" Tag "] " __VA_ARGS__)
#define LOG_CRITICAL(...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__)
#define LOG_CRITICAL_TAG(Tag, ...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, "[" Tag "] " __VA_ARGS__)
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

template <Traits::IToString T> std::ostream& operator<<(std::ostream& OS, const T& Str) {
  OS << Str.ToString();
  return OS;
}

template <Traits::IToString T> struct fmt::formatter<T> : ostream_formatter {};

