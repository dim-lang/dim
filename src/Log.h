// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <cstdio>
#include <string>

class Log {
public:
  static void initialize(const std::string &logPath = ".",
                         const std::string &logName = "fastype",
                         bool debug = false);
};

#ifdef NDEBUG

#define FDEBUG_MSG(msg)
#define FDEBUG(msg, ...)
#define FINFO_MSG(msg)
#define FINFO(msg, ...)
#define FCHECK_MSG(cond, msg)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      throw fmt::format("Check Fail! {}:{} {} - Condition: {}, Result: {}",    \
                        __FILE__, __LINE__, __FUNCTION__,                      \
                        BOOST_PP_STRINGIZE(cond), msg)                         \
    }                                                                          \
  } while (0)

#define FCHECK(cond, msg, ...)                                                 \
  do {                                                                         \
    if (!(cond)) {                                                             \
      throw fmt::format("Check Fail! {}:{} {} - Condition: {}, Result: {}",    \
                        __FILE__, __LINE__, __FUNCTION__,                      \
                        BOOST_PP_STRINGIZE(cond),                              \
                        fmt::format(msg, __VA_ARGS__))                         \
    }                                                                          \
  } while (0)

#else

#define FDEBUG_MSG(msg) SPDLOG_DEBUG(msg)
#define FDEBUG(msg, ...) SPDLOG_DEBUG(msg, __VA_ARGS__)
#define FINFO_MSG(msg) SPDLOG_INFO(msg)
#define FINFO(msg, ...) SPDLOG_INFO(msg, __VA_ARGS__)
#define FCHECK_MSG(cond, msg)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::fprintf(                                                            \
          stderr, "Check Fail! %s:%d %s - Condition: %s, Result: %s\n",        \
          __FILE__, __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), msg);    \
    }                                                                          \
    BOOST_ASSERT(cond);                                                        \
  } while (0)
#define FCHECK(cond, msg, ...)                                                 \
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

#define FERROR_MSG(msg) SPDLOG_ERROR(msg)
#define FERROR(msg, ...) SPDLOG_ERROR(msg, __VA_ARGS__)
