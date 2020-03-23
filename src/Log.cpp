// Copyright 2019- <collie.org>
// Apache License Version 2.0

#include "Log.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <ctime>
#include <string>

void Log::initialize(const std::string &logName, const std::string &logPath) {
#ifndef NDEBUG
  spdlog::set_level(spdlog::level::debug);
#else
  spdlog::set_level(spdlog::level::err);
#endif

  spdlog::set_pattern(
      "%Y-%m-%d %H:%M:%S.%e [%n] %l process-%P thread-%t [%@ %!] %v");

  std::time_t timestamp = std::time(nullptr);
  std::tm dt = *std::localtime(&timestamp);
  std::string fileName =
      fmt::format("{}/{}-{:04d}.{:02d}.{:02d}-{:02d}.{:02d}.{:02d}.log",
                  logPath, logName, dt.tm_year + 1900, dt.tm_mon + 1,
                  dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);

  spdlog::set_default_logger(spdlog::basic_logger_mt(logName, fileName));
}
