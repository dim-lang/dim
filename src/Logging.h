// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "spdlog/spdlog.h"
#include <cstdio>
#include <memory>
#include <string>

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

class Logging {
public:
  static void initialize(const std::string &fileName);

  template <typename... Args>
  static void debug(const detail::Location &location, const char *fmtMsg,
                    const Args &... args) {
    std::string formattedMsg = detail::FormatLocation(location, fmtMsg);
    spdlog::debug(formattedMsg.data(), args...);
  }

  template <typename... Args>
  static void info(const detail::Location &location, const char *fmtMsg,
                   const Args &... args) {
    std::string formattedMsg = detail::FormatLocation(location, fmtMsg);
    spdlog::info(formattedMsg.data(), args...);
  }

  template <typename... Args>
  static void warn(const detail::Location &location, const char *fmtMsg,
                   const Args &... args) {
    std::string formattedMsg = detail::FormatLocation(location, fmtMsg);
    spdlog::warn(formattedMsg.data(), args...);
  }

  template <typename... Args>
  static void error(const detail::Location &location, const char *fmtMsg,
                    const Args &... args) {
    std::string formattedMsg = detail::FormatLocation(location, fmtMsg);
    spdlog::error(formattedMsg.data(), args...);
  }
};

} // namespace fastype

#ifndef F_LOG_LOCATION
#define F_LOG_LOCATION                                                         \
  fastype::detail::Location(__FILE__, __LINE__, __FUNCTION__)
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
    fastype::Logging::debug(F_LOG_LOCATION, msg);                              \
  } while (0)
#endif

#ifndef F_DEBUGF
#define F_DEBUGF(fmtMsg, ...)                                                  \
  do {                                                                         \
    fastype::Logging::debug(F_LOG_LOCATION, fmtMsg, __VA_ARGS__);              \
  } while (0)
#endif

#ifndef F_INFOF
#define F_INFOF(fmtMsg, ...)                                                   \
  do {                                                                         \
    fastype::Logging::info(F_LOG_LOCATION, fmtMsg, __VA_ARGS__);               \
  } while (0)
#endif

#ifndef F_INFO
#define F_INFO(msg)                                                            \
  do {                                                                         \
    fastype::Logging::info(F_LOG_LOCATION, msg);                               \
  } while (0)
#endif

#endif // #ifdef NDEBUG

#ifndef F_ERRORF
#define F_ERRORF(fmtMsg, ...)                                                  \
  do {                                                                         \
    fastype::Logging::error(F_LOG_LOCATION, fmtMsg, __VA_ARGS__);              \
  } while (0)
#endif

#ifndef F_ERROR
#define F_ERROR(msg)                                                           \
  do {                                                                         \
    fastype::Logging::error(F_LOG_LOCATION, msg);                              \
  } while (0)
#endif
