// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <cstdio>
#include <memory>
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

#define F_LOG_INIT(p, n) fastype::Logging::initialize(p, n)

#ifdef NDEBUG

#define F_DEBUG(msg)
#define F_DEBUGF(fmtMsg, ...)
#define F_INFO(msg)
#define F_INFOF(fmtMsg, ...)

#else

#define F_DEBUG(msg) SPDLOG_DEBUG(msg)
#define F_DEBUGF(fmtMsg, ...) SPDLOG_DEBUG(fmtMsg, __VA_ARGS__)
#define F_INFO(msg) SPDLOG_INFO(msg)
#define F_INFOF(fmtMsg, ...) SPDLOG_INFO(fmtMsg, __VA_ARGS__)

#endif // #ifdef NDEBUG

#define F_ERROR(msg) SPDLOG_ERROR(msg)
#define F_ERRORF(fmtMsg, ...) SPDLOG_ERROR(fmtMsg, __VA_ARGS__)
