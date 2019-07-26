// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "exceptions/PreCheckException.h"
#include "spdlog/spdlog.h"
#include <cstdio>
#include <memory>
#include <mutex>
#include <stdio.h>
#include <string>
#include <thread>

namespace fastype {

namespace detail {

// location for file name, line number, function name
class Location : public Stringify {
public:
  Location(const char *fileName, int lineNumber, const char *functionName);
  virtual ~Location() = default;

  const std::string &fileName() const;
  int lineNumber() const;
  const std::string &functionName() const;
  virtual std::string toString() const;

private:
  std::string fileName_;
  int lineNumber_;
  std::string functionName_;
};

template <typename... Args>
inline void CheckCondition(bool cond, const char *fmt, const Args &... args) {
  if (cond) {
    return;
  }
  throw fastype::PreCheckException(fmt::format(fmt, args...));
}

std::string FormatLocation(const Location &location, const char *fmt);

} // namespace detail

class LogManager;

class Logger {
public:
  virtual ~Logger() = default;

  template <typename... Args>
  inline void debug(const detail::Location &location, const char *fmt,
                    const Args &... args) {
    std::string rfmt = detail::FormatLocation(location, fmt);
    logger->debug(rfmt.data(), args...);
  }

  template <typename... Args>
  inline void info(const detail::Location &location, const char *fmt,
                   const Args &... args) {
    std::string rfmt = detail::FormatLocation(location, fmt);
    logger->info(rfmt.data(), args...);
  }

  template <typename... Args>
  inline void warn(const detail::Location &location, const char *fmt,
                   const Args &... args) {
    std::string rfmt = detail::FormatLocation(location, fmt);
    logger->warn(rfmt.data(), args...);
  }

  template <typename... Args>
  inline void error(const detail::Location &location, const char *fmt,
                    const Args &... args) {
    std::string rfmt = detail::FormatLocation(location, fmt);
    logger->error(rfmt.data(), args...);
  }

private:
  friend class LogManager;

  explicit Logger(std::shared_ptr<spdlog::logger> logger) : logger(logger) {}

  std::shared_ptr<spdlog::logger> logger;
};

class LogManager {
public:
  virtual ~LogManager() = default;
  static std::shared_ptr<Logger> getLogger(const std::string &loggerName);
};

class Logging {
public:
  Logging(const std::string &loggingName)
      : logging_(LogManager::getLogger(loggingName)) {}

protected:
  std::shared_ptr<Logger> logging_;
};

} // namespace fastype

#ifndef LOG_LOCATION
#define LOG_LOCATION fastype::detail::Location(__FILE__, __LINE__, __FUNCTION__)
#endif

#ifdef NDEBUG

#ifndef F_DEBUGF
#define F_DEBUGF(fmt, ...)
#endif
#ifndef F_DEBUG
#define F_DEBUG(msg)
#endif
#ifndef F_INFOF
#define F_INFOF(fmt, ...)
#endif
#ifndef F_INFO
#define F_INFO(msg)
#endif
#ifndef F_CHECK
#define F_CHECK(cond, msg)
#endif
#ifndef F_CHECKF
#define F_CHECKF(cond, fmt, ...)
#endif

#else

#ifndef F_DEBUG
#define F_DEBUG(msg)                                                           \
  do {                                                                         \
    (logging_)->debug(LOG_LOCATION, msg);                                      \
  } while (0)
#endif

#ifndef F_DEBUGF
#define F_DEBUGF(fmt, ...)                                                     \
  do {                                                                         \
    (logging_)->debug(LOG_LOCATION, fmt, __VA_ARGS__);                         \
  } while (0)
#endif

#ifndef F_INFOF
#define F_INFOF(fmt, ...)                                                      \
  do {                                                                         \
    (logging_)->info(LOG_LOCATION, fmt, __VA_ARGS__);                          \
  } while (0)
#endif

#ifndef F_INFO
#define F_INFO(msg)                                                            \
  do {                                                                         \
    (logging_)->info(LOG_LOCATION, msg);                                       \
  } while (0)
#endif

#ifndef F_CHECK
#define F_CHECK(cond, msg)                                                     \
  do {                                                                         \
    std::string metaMsg = fastype::detail::FormatLocation(LOG_LOCATION, fmt);  \
    fastype::detail::CheckCondition(cond, metaMsg.data());                     \
  } while (0)
#endif

#ifndef F_CHECKF
#define F_CHECKF(cond, fmt, ...)                                               \
  do {                                                                         \
    std::string metaMsg = fastype::detail::FormatLocation(LOG_LOCATION, fmt);  \
    fastype::detail::CheckCondition(cond, metaMsg.data(), __VA_ARGS__);        \
  } while (0)
#endif

#endif // #ifdef NDEBUG

#ifndef F_ERRORF
#define F_ERRORF(fmt, ...)                                                     \
  do {                                                                         \
    (logging_)->error(LOG_LOCATION, fmt, __VA_ARGS__);                         \
  } while (0)
#endif

#ifndef F_ERROR
#define F_ERROR(msg)                                                           \
  do {                                                                         \
    (logging_)->error(LOG_LOCATION, msg);                                      \
  } while (0)
#endif
