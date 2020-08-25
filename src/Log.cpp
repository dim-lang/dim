// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void Log::initialize(const std::string &logName) {
#ifdef NDEBUG
  spdlog::set_level(spdlog::level::error);
#else
  spdlog::set_level(spdlog::level::debug);
#endif

  spdlog::set_pattern(
      "%Y-%m-%d %H:%M:%S.%e [%n] %l process-%P thread-%t [%@ %!] %v");
  spdlog::set_default_logger(spdlog::stdout_color_mt(logName));
}
