// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "spdlog/spdlog.h"
#include <cstdio>
#include <memory>
#include <string>

namespace fastype {

class Logging {
public:
  static void initialize(const std::string &logPath,
                         const std::string &logName);
};

} // namespace fastype

#ifndef F_DETAIL_LOG_LOCATION
#define F_DETAIL_LOG_LOCATION                                                  \
  fastype::detail::LogLocation(__FILE__, __LINE__, __FUNCTION__)
#endif

#ifndef F_LOG_INIT
#define F_LOG_INIT(p, n) fastype::Logging::initialize(p, n)
#endif

#ifdef NDEBUG

#ifndef F_DEBUG
#define F_DEBUG(msg)
#endif
#ifndef F_DEBUGF
#define F_DEBUGF(fmtMsg, ...)
#endif
#ifndef F_INFO
#define F_INFO(msg)
#endif
#ifndef F_INFOF
#define F_INFOF(fmtMsg, ...)
#endif

#else

#ifndef F_DEBUG
#define F_DEBUG(msg) SPDLOG_DEBUG(msg)
#endif

#ifndef F_DEBUGF
#define F_DEBUGF(fmtMsg, ...) SPDLOG_DEBUG(fmtMsg, __VA_ARGS__)
#endif

#ifndef F_INFO
#define F_INFO(msg) SPDLOG_INFO(msg)
#endif

#ifndef F_INFOF
#define F_INFOF(fmtMsg, ...) SPDLOG_INFO(fmtMsg, __VA_ARGS__)
#endif

#endif // #ifdef NDEBUG

#ifndef F_ERROR
#define F_ERROR(msg) SPDLOG_ERROR(msg)
#endif

#ifndef F_ERRORF
#define F_ERRORF(fmtMsg, ...) SPDLOG_ERROR(fmtMsg, __VA_ARGS__)
#endif
