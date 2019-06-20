// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include "Util.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace fastype {

std::string Logger::formatLocation(const LogLocation &location,
                                   const char *fmt) {
  std::string shortFileName(location.fileName);
  size_t slashPos = shortFileName.find_last_of("/");
  slashPos = slashPos != std::string::npos ? slashPos
                                           : shortFileName.find_last_of("\\");
  if (slashPos != std::string::npos) {
    shortFileName = shortFileName.substr(slashPos + 1);
  }
  return std::string(std::string("[") + shortFileName + "] [" +
                     location.functionName + ":" +
                     std::to_string(location.lineNumber) + "] " + fmt);
}

static std::unordered_map<std::string, std::shared_ptr<Logger>> LoggerMap =
    std::unordered_map<std::string, std::shared_ptr<Logger>>();
static std::string FileName;
static const int MaxFileSize = 1048576 * 10;
static const int MaxFiles = 100;

std::shared_ptr<Logger> LogManager::getLogger(const std::string &loggerName) {
  if (LoggerMap.find(loggerName) == LoggerMap.end()) {
    std::shared_ptr<spdlog::logger> spdLogger =
        spdlog::rotating_logger_mt(loggerName, FileName, MaxFileSize, MaxFiles);
    LoggerMap.insert(std::make_pair(
        loggerName, std::shared_ptr<Logger>(new Logger(spdLogger))));
  }
  return LoggerMap[loggerName];
}

F_STATIC_BEGIN(Log)
spdlog::set_level(spdlog::level::debug);
spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e %z] [%n] [%l] [thread %t] %v");
FileName = std::string("fastype-") +
           fastype::formatDatetime(fastype::nowTimestamp(), "%Y%m%d-%H%M%S") +
           ".log";
F_STATIC_END(Log)

} // namespace fastype
