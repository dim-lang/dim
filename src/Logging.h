// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "exception/CheckFailureException.h"
#include <boost/assert.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <cstdio>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <string>

namespace fastype {

class Logging {
public:
  static void initialize(const std::string &logPath = ".",
                         const std::string &logName = "fastype",
                         bool debug = false);
};

} // namespace fastype

#ifdef NDEBUG

#define F_DEBUG_MSG(msg)
#define F_DEBUG(msg, ...)
#define F_INFO_MSG(msg)
#define F_INFO(msg, ...)
#define F_CHECK_MSG(cond, msg)
#define F_CHECK(cond, msg, ...)

#else

#define F_DEBUG_MSG(msg) SPDLOG_DEBUG(msg)
#define F_DEBUG(msg, ...) SPDLOG_DEBUG(msg, __VA_ARGS__)
#define F_INFO_MSG(msg) SPDLOG_INFO(msg)
#define F_INFO(msg, ...) SPDLOG_INFO(msg, __VA_ARGS__)
#define F_CHECK_MSG(cond, msg)                                                 \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::fprintf(                                                            \
          stderr, "Check Fail! %s:%d %s - Condition: %s, Result: %s\n",        \
          __FILE__, __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), msg);    \
    }                                                                          \
    BOOST_ASSERT(cond);                                                        \
  } while (0)
#define F_CHECK(cond, msg, ...)                                                \
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

#define F_ERROR_MSG(msg) SPDLOG_ERROR(msg)
#define F_ERROR(msg, ...) SPDLOG_ERROR(msg, __VA_ARGS__)
