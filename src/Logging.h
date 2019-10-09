// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <boost/assert.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <cstdio>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <string>

namespace fastype {

class Logging {
public:
  static void initialize(const std::string &logPath,
                         const std::string &logName);
};

} // namespace fastype

#define F_DETAIL_LOG_LOCATION                                                  \
  fastype::detail::LogLocation(__FILE__, __LINE__, __FUNCTION__)

#ifdef NDEBUG

#define F_DEBUG(msg)
#define F_DEBUGF(fmtMsg, ...)
#define F_INFO(msg)
#define F_INFOF(fmtMsg, ...)
#define F_INFO(msg)
#define F_INFOF(fmtMsg, ...)
#define F_CHECK(cond, msg)
#define F_CHECKF(cond, fmtMsg, ...)

#else

#define F_DEBUG(msg) SPDLOG_DEBUG(msg)
#define F_DEBUGF(fmtMsg, ...) SPDLOG_DEBUG(fmtMsg, __VA_ARGS__)
#define F_INFO(msg) SPDLOG_INFO(msg)
#define F_INFOF(fmtMsg, ...) SPDLOG_INFO(fmtMsg, __VA_ARGS__)
#define F_CHECK(cond, msg)                                                     \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::fprintf(                                                            \
          stderr, "Check Fail! %s:%d %s - Condition: %s, Result: %s\n",        \
          __FILE__, __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), msg);    \
    }                                                                          \
    BOOST_ASSERT(cond);                                                        \
  } while (0)
#define F_CHECKF(cond, fmtMsg, ...)                                            \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::string msg = fmt::format(fmtMsg, __VA_ARGS__);                      \
      std::fprintf(stderr,                                                     \
                   "Check Fail! %s:%d %s - Condition: %s, Result: %s\n",       \
                   __FILE__, __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), \
                   msg.data());                                                \
    }                                                                          \
    BOOST_ASSERT(cond);                                                        \
  } while (0)

#endif // #ifdef NDEBUG

#define F_ERROR(msg) SPDLOG_ERROR(msg)
#define F_ERRORF(fmtMsg, ...) SPDLOG_ERROR(fmtMsg, __VA_ARGS__)
