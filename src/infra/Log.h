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

// check

#define CHECK_PRINT(cond, ...)                                                 \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fmt::print(stdout, __VA_ARGS__);                                         \
    }                                                                          \
  } while (0)

#define CHECK_PRINT_ERROR(cond, ...)                                           \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fmt::print(stdout, "error: {}\n", fmt::format(__VA_ARGS__));             \
    }                                                                          \
  } while (0)

#define CHECK_PRINT_WARN(cond, ...)                                            \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fmt::print(stdout, "warn: {}\n", fmt::format(__VA_ARGS__));              \
    }                                                                          \
  } while (0)

#define CHECK_THROW(cond, ...)                                                 \
  do {                                                                         \
    if (!(cond)) {                                                             \
      throw Exception(__FILE__, __LINE__, __FUNCTION__,                        \
                      fmt::format(__VA_ARGS__));                               \
    }                                                                          \
  } while (0)

#define CHECK_THROW_ERROR(cond, ...)                                           \
  do {                                                                         \
    if (!(cond)) {                                                             \
      throw Exception(__FILE__, __LINE__, __FUNCTION__,                        \
                      fmt::format("error: {}\n", fmt::format(__VA_ARGS__)));   \
    }                                                                          \
  } while (0)

// log

#define PRINT(...) fmt::print(stdout, __VA_ARGS__)
#define PRINT_ERROR(...)                                                       \
  fmt::print(stdout, "error: {}\n", fmt::format(__VA_ARGS__))
#define PRINT_WARN(...)                                                        \
  fmt::print(stdout, "warn: {}\n", fmt::format(__VA_ARGS__))

#define THROW(...)                                                             \
  throw Exception(__FILE__, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define THROW_ERROR(...)                                                       \
  throw Exception(__FILE__, __LINE__, __FUNCTION__,                            \
                  fmt::format("error: {}\n", fmt::format(__VA_ARGS__)))

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
      throw Exception(__FILE__, __LINE__, __FUNCTION__, msg);                  \
    }                                                                          \
  } while (0)
