// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace fastype {

static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>>
    LoggerMap =
        std::unordered_map<std::string, std::shared_ptr<spdlog::logger>>();
static const char *FileName = "fastype.log";
static const int MaxFileSize = 1048576 * 10;
static const int MaxFiles = 100;

std::shared_ptr<spdlog::logger>
LoggerManager::getLogger(const std::string &loggerName) {
  if (LoggerMap.find(loggerName) == LoggerMap.end()) {
    std::shared_ptr<spdlog::logger> logger =
        spdlog::rotating_logger_mt(loggerName, FileName, MaxFileSize, MaxFiles);
    LoggerMap.insert(std::make_pair(loggerName, logger));
  }

  return LoggerMap[loggerName];
}

} // namespace fastype
