// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/preprocessor/stringize.hpp"
#include "fmt/format.h"
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
#define LOG_ASSERT(cond, ...)

#else

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_ASSERT(cond, ...)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::string msg = fmt::format(__VA_ARGS__);                              \
      fmt::print("Assert Fail! {}:{} {} - Condition:{}, Result:{}\n",          \
                 __FILE__, __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond),   \
                 msg);                                                         \
      throw fmt::format("Assert Fail! {}:{} {} - Condition:{}, Result:{}\n",   \
                        __FILE__, __LINE__, __FUNCTION__,                      \
                        BOOST_PP_STRINGIZE(cond), msg);                        \
    }                                                                          \
  } while (0)

#endif // #ifdef NDEBUG

#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
