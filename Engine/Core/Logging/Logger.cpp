//
// Created by kita on 25-7-24.
//

#include "Logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

Logger::Logger() : mLogger("") {
  auto ConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  ConsoleSink->set_pattern("%^[%T] [%L] [%t] %v%$");
  mLogger = spdlog::logger{"", {ConsoleSink}};
}