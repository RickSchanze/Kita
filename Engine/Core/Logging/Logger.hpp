#pragma once

#include "spdlog/spdlog.h"

#include "Core/Macros.h"

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
#define LOG_ERROR(...)                                                                                                                                                                                 \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__);                                                                                   \
  DEBUG_BREAK()

#define LOG_CRITICAL(...)                                                                                                                                                                              \
  gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__);                                                                              \
  DEBUG_BREAK()

#else
#define LOG_ERROR(...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(...) gLogger.GetLogger().log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__)
#endif