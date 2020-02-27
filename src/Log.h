// Copyright 2019- <coli-lang>
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
  static void initialize(const std::string &logName = "coli",
                         const std::string &logPath = ".");
};

#ifdef NDEBUG

#define CTRACE(...)
#define CDEBUG(...)
#define CINFO(...)
#define CASSERT(cond, ...)

#else

#define CTRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define CDEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define CINFO(...) SPDLOG_INFO(__VA_ARGS__)
#define CASSERT(cond, ...)                                                     \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::fprintf(stderr,                                                     \
                   "Assert Fail! %s:%d %s - Condition: %s, Result: %s\n",      \
                   __FILE__, __LINE__, __FUNCTION__, BOOST_PP_STRINGIZE(cond), \
                   fmt::format(__VA_ARGS__).c_str());                          \
    }                                                                          \
    assert((cond) &&                                                           \
           (fmt::format("Assert Fail! {}:{} {} - Condition: {}, Result: {}",   \
                        __FILE__, __LINE__, __FUNCTION__,                      \
                        BOOST_PP_STRINGIZE(cond), fmt::format(__VA_ARGS__))    \
                .c_str()));                                                    \
  } while (0)

#endif // #ifdef NDEBUG

#define CERROR(...) SPDLOG_ERROR(__VA_ARGS__)
