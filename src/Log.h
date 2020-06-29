// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "spdlog/spdlog.h"
#include <cassert>
#include <cstdio>
#include <string>

class Log {
public:
  static void initialize(const std::string &logName = "shepherd",
                         const std::string &logPath = ".");
};

#ifdef NDEBUG

#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)

#else

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)

#endif // #ifdef NDEBUG

#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
