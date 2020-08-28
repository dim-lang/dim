// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

#ifdef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_ERROR
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif // #ifdef NDEBUG

#include "boost/preprocessor/stringize.hpp"
#include "fmt/format.h"
#include "spdlog/spdlog.h"
#include <string>

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_ASSERT(cond, ...)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::string msg1 = fmt::format(__VA_ARGS__);                             \
      LOG_ERROR("Assert Fail! {}:{} {} - Condition:{}, Result:{}\n", __FILE__, \
                __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), msg1);       \
      throw fmt::format("Assert Fail! {}:{} {} - Condition:{}, Result:{}\n",   \
                        __FILE__, __LINE__, __FUNCTION__,                      \
                        BOOST_PP_STRINGIZE(cond), msg1);                       \
    }                                                                          \
  } while (0)

class Log {
public:
  static void initialize(const std::string &name = "nerd");
};
