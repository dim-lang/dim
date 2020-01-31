// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <cstdio>
#include <string>

class Log {
public:
  static void initialize(const std::string &logPath = ".",
                         const std::string &logName = "coli",
                         bool debug = false);
};

#ifdef NDEBUG

#define LOG_DEBUG_MSG(msg)
#define LOG_DEBUG(msg, ...)
#define LOG_INFO_MSG(msg)
#define LOG_INFO(msg, ...)
#define LOG_CHECK_MSG(cond, msg)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      throw fmt::format("Check Fail! {}:{} {} - Condition: {}, Result: {}",    \
                        __FILE__, __LINE__, __FUNCTION__,                      \
                        BOOST_PP_STRINGIZE(cond), msg)                         \
    }                                                                          \
  } while (0)

#define LOG_CHECK(cond, msg, ...)                                                 \
  do {                                                                         \
    if (!(cond)) {                                                             \
      throw fmt::format("Check Fail! {}:{} {} - Condition: {}, Result: {}",    \
                        __FILE__, __LINE__, __FUNCTION__,                      \
                        BOOST_PP_STRINGIZE(cond),                              \
                        fmt::format(msg, __VA_ARGS__))                         \
    }                                                                          \
  } while (0)

#else

#define LOG_DEBUG_MSG(msg) SPDLOG_DEBUG(msg)
#define LOG_DEBUG(msg, ...) SPDLOG_DEBUG(msg, __VA_ARGS__)
#define LOG_INFO_MSG(msg) SPDLOG_INFO(msg)
#define LOG_INFO(msg, ...) SPDLOG_INFO(msg, __VA_ARGS__)
#define LOG_CHECK_MSG(cond, msg)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::fprintf(                                                            \
          stderr, "Check Fail! %s:%d %s - Condition: %s, Result: %s\n",        \
          __FILE__, __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), msg);    \
    }                                                                          \
    BOOST_ASSERT(cond);                                                        \
  } while (0)
#define LOG_CHECK(cond, msg, ...)                                                 \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::fprintf(stderr,                                                     \
                   "Check Fail! %s:%d %s - Condition: %s, Result: %s\n",       \
                   __FILE__, __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), \
                   fmt::format(msg, __VA_ARGS__).data());                      \
    }                                                                          \
    BOOST_ASSERT(cond);                                                        \
  } while (0)

#endif // #ifdef NDEBUG

#define LOG_ERROR_MSG(msg) SPDLOG_ERROR(msg)
#define LOG_ERROR(msg, ...) SPDLOG_ERROR(msg, __VA_ARGS__)
