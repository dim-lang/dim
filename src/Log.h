// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "spdlog/spdlog.h"
#include <cstdio>
#include <memory>
#include <mutex>
#include <stdio.h>
#include <string>
#include <thread>

namespace fastype {

namespace detail {

class LogLocation {
public:
  LogLocation(const char *fileName, int lineNumber, const char *functionName)
      : fileName(fileName), lineNumber(lineNumber), functionName(functionName) {
  }
  virtual ~LogLocation() = default;

  std::string fileName;
  int lineNumber;
  std::string functionName;
};

} // namespace detail

class LogManager;

class Logger {
public:
  virtual ~Logger() = default;

  template <typename... Args>
  inline void debug(const detail::LogLocation &location, const char *fmt,
                    const Args &... args) {
    std::string rfmt = formatLocation(location, fmt);
    logger->debug(rfmt.data(), args...);
  }

  template <typename... Args>
  inline void info(const detail::LogLocation &location, const char *fmt,
                   const Args &... args) {
    std::string rfmt = formatLocation(location, fmt);
    logger->info(rfmt.data(), args...);
  }

  template <typename... Args>
  inline void warn(const detail::LogLocation &location, const char *fmt,
                   const Args &... args) {
    std::string rfmt = formatLocation(location, fmt);
    logger->warn(rfmt.data(), args...);
  }

  template <typename... Args>
  inline void error(const detail::LogLocation &location, const char *fmt,
                    const Args &... args) {
    std::string rfmt = formatLocation(location, fmt);
    logger->error(rfmt.data(), args...);
  }

private:
  friend class LogManager;

  explicit Logger(std::shared_ptr<spdlog::logger> logger) : logger(logger) {}

  std::string formatLocation(const detail::LogLocation &location,
                             const char *fmt);

  std::shared_ptr<spdlog::logger> logger;
};

class LogManager {
public:
  virtual ~LogManager() = default;
  static std::shared_ptr<Logger> getLogger(const std::string &loggerName);
};

} // namespace fastype

#ifdef NDEBUG

#ifndef F_DEBUGF
#define F_DEBUGF(logger, fmt, ...)
#endif

#ifndef F_DEBUG
#define F_DEBUG(logger, msg)
#endif

#ifndef F_INFOF
#define F_INFOF(logger, fmt, ...)
#endif

#ifndef F_INFO
#define F_INFO(logger, msg)
#endif

#else

#ifndef F_DEBUGF
#define F_DEBUGF(logger, fmt, ...)                                             \
  do {                                                                         \
    (logger)->debug(                                                           \
        fastype::detail::LogLocation(__FILE__, __LINE__, __FUNCTION__), fmt,   \
        __VA_ARGS__);                                                          \
  } while (0)
#endif

#ifndef F_DEBUG
#define F_DEBUG(logger, msg)                                                   \
  do {                                                                         \
    (logger)->debug(                                                           \
        fastype::detail::LogLocation(__FILE__, __LINE__, __FUNCTION__), msg);  \
  } while (0)
#endif

#ifndef F_INFOF
#define F_INFOF(logger, fmt, ...)                                              \
  do {                                                                         \
    (logger)->info(                                                            \
        fastype::detail::LogLocation(__FILE__, __LINE__, __FUNCTION__), fmt,   \
        __VA_ARGS__);                                                          \
  } while (0)
#endif

#ifndef F_INFO
#define F_INFO(logger, msg)                                                    \
  do {                                                                         \
    (logger)->info(                                                            \
        fastype::detail::LogLocation(__FILE__, __LINE__, __FUNCTION__), msg);  \
  } while (0)
#endif

#endif // #ifdef NDEBUG

#ifndef F_ERRORF
#define F_ERRORF(logger, fmt, ...)                                             \
  do {                                                                         \
    (logger)->error(                                                           \
        fastype::detail::LogLocation(__FILE__, __LINE__, __FUNCTION__), fmt,   \
        __VA_ARGS__);                                                          \
  } while (0)
#endif

#ifndef F_ERROR
#define F_ERROR(logger, msg)                                                   \
  do {                                                                         \
    (logger)->error(fastype::LogLocation(__FILE__, __LINE__, __FUNCTION__),    \
                    msg);                                                      \
  } while (0)
#endif
