// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Log.h"
#include <ctime>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <string>

void Log::initialize(const std::string &logPath, const std::string &logName,
                     bool debug) {
#ifndef NDEBUG
  debug = true;
#endif

  spdlog::set_level(debug ? spdlog::level::debug : spdlog::level::err);
  spdlog::set_pattern(
      "%Y-%m-%d %H:%M:%S.%e [%n] %l process-%P thread-%t [%@ %!] %v");

  std::time_t timestamp = std::time(nullptr);
  std::tm dt = *std::localtime(&timestamp);
  std::string fileName =
      fmt::format("{}/{}-{:04d}-{:02d}-{:02d}-{:02d}-{:02d}-{:02d}.log",
                  logPath, logName, dt.tm_year + 1900, dt.tm_mon + 1,
                  dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
  spdlog::set_default_logger(spdlog::basic_logger_mt(logName, fileName));
}
