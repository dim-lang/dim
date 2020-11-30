// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once

#ifdef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_ERROR
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif // #ifdef NDEBUG

#include "boost/preprocessor/stringize.hpp"
#include "fmt/format.h"
#include "infra/Cowstr.h"
#include "infra/Log.h"
#include "spdlog/spdlog.h"
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <stdexcept>

class Exception : public std::exception {
public:
  Exception(const Cowstr &fileName, int lineNumber, const Cowstr &function,
            const Cowstr &message);
  virtual ~Exception() = default;
  virtual const char *what() const noexcept;
  virtual const Cowstr &message() const;
  virtual const Cowstr &fileName() const;
  virtual const int &lineNumber() const;
  virtual const Cowstr &function() const;

protected:
  Cowstr message_;
  Cowstr fileName_;
  int lineNumber_;
  Cowstr function_;
};

// print

#undef PRINT
#define PRINT(...) fmt::print(__VA_ARGS__)

#undef PRINT_IF
#define PRINT_IF(cond, ...)                                                    \
  do {                                                                         \
    if (cond) {                                                                \
      fmt::print(__VA_ARGS__);                                         \
    }                                                                          \
  } while (0)

// assert

#undef ERROR
#define ERROR(...)                                                              \
  throw Exception(__FILE__, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#undef ASSERT
#define ASSERT(cond, ...)                                                \
  do {                                                                         \
    if (!(cond)) {                                                             \
      throw Exception(__FILE__, __LINE__, __FUNCTION__,                        \
                      fmt::format(__VA_ARGS__));                               \
    }                                                                          \
  } while (0)

#undef ASSERT_MSG
#define ASSERT_MSG(cond, ...)                                                \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fmt::print(stderr, __VA_ARGS__);                                         \
    }                                                                          \
  } while (0)

// file log

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)

#define LOG_ASSERT(cond, ...)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      Cowstr msg =                                                             \
          fmt::format("Assert Fail! {}:{} {} - Condition:{}, Result:{}\n",     \
                      __FILE__, __LINE__, __FUNCTION__,                        \
                      BOOST_PP_STRINGIZE(cond), fmt::format(__VA_ARGS__));     \
      fmt::print(stderr, "{}", msg);                                           \
      LOG_ERROR("{}", msg);                                                    \
      ERROR("{}", msg);                                                        \
    }                                                                          \
  } while (0)
