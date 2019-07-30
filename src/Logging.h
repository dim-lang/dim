// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "spdlog/spdlog.h"
#include <cstdio>
#include <memory>
#include <mutex>
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

std::string FormatLocation(const Location &location, const char *fmtMsg);

} // namespace detail

class LogManager;

class Logger {
public:
  virtual ~Logger() = default;

  template <typename... Args>
  inline void debug(const detail::Location &location, const char *fmtMsg,
                    const Args &... args) {
    std::string formattedMsg = detail::FormatLocation(location, fmtMsg);
    logger->debug(formattedMsg.data(), args...);
  }

  template <typename... Args>
  inline void info(const detail::Location &location, const char *fmtMsg,
                   const Args &... args) {
    std::string formattedMsg = detail::FormatLocation(location, fmtMsg);
    logger->info(formattedMsg.data(), args...);
  }

  template <typename... Args>
  inline void warn(const detail::Location &location, const char *fmtMsg,
                   const Args &... args) {
    std::string formattedMsg = detail::FormatLocation(location, fmtMsg);
    logger->warn(formattedMsg.data(), args...);
  }

  template <typename... Args>
  inline void error(const detail::Location &location, const char *fmtMsg,
                    const Args &... args) {
    std::string formattedMsg = detail::FormatLocation(location, fmtMsg);
    logger->error(formattedMsg.data(), args...);
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

#ifndef F_LOG_LOCATION
#define F_LOG_LOCATION                                                         \
  fastype::detail::Location(__FILE__, __LINE__, __FUNCTION__)
#endif

#ifndef F_LOGGER
#define F_LOGGER(x) auto logging_ = LogManager::getLogger(x)
#endif

#ifdef NDEBUG

#ifndef F_DEBUGF
#define F_DEBUGF(fmtMsg, ...)
#endif
#ifndef F_DEBUG
#define F_DEBUG(msg)
#endif
#ifndef F_INFOF
#define F_INFOF(fmtMsg, ...)
#endif
#ifndef F_INFO
#define F_INFO(msg)
#endif

#else

#ifndef F_DEBUG
#define F_DEBUG(msg)                                                           \
  do {                                                                         \
    (logging_)->debug(F_LOG_LOCATION, msg);                                    \
  } while (0)
#endif

#ifndef F_DEBUGF
#define F_DEBUGF(fmtMsg, ...)                                                  \
  do {                                                                         \
    (logging_)->debug(F_LOG_LOCATION, fmtMsg, __VA_ARGS__);                    \
  } while (0)
#endif

#ifndef F_INFOF
#define F_INFOF(fmtMsg, ...)                                                   \
  do {                                                                         \
    (logging_)->info(F_LOG_LOCATION, fmtMsg, __VA_ARGS__);                     \
  } while (0)
#endif

#ifndef F_INFO
#define F_INFO(msg)                                                            \
  do {                                                                         \
    (logging_)->info(F_LOG_LOCATION, msg);                                     \
  } while (0)
#endif

#endif // #ifdef NDEBUG

#ifndef F_ERRORF
#define F_ERRORF(fmtMsg, ...)                                                  \
  do {                                                                         \
    (logging_)->error(F_LOG_LOCATION, fmtMsg, __VA_ARGS__);                    \
  } while (0)
#endif

#ifndef F_ERROR
#define F_ERROR(msg)                                                           \
  do {                                                                         \
    (logging_)->error(F_LOG_LOCATION, msg);                                    \
  } while (0)
#endif
