// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include "Util.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <mutex>
#include <string>
#include <thread>
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

static std::mutex LoggerLock;
static std::unordered_map<std::string, std::shared_ptr<Logger>> LoggerMap =
    std::unordered_map<std::string, std::shared_ptr<Logger>>();
static const std::string FileName = "fastype.log";
static const int MaxFileSize = 1048576 * 10;
static const int MaxFiles = 100;

std::shared_ptr<Logger> LogManager::getLogger(const std::string &loggerName) {
  std::lock_guard<std::mutex> guard(LoggerLock);
  if (LoggerMap.find(loggerName) == LoggerMap.end()) {
    std::shared_ptr<spdlog::logger> spdLogger =
        spdlog::rotating_logger_mt(loggerName, FileName, MaxFileSize, MaxFiles);
    LoggerMap.insert(std::make_pair(
        loggerName, std::shared_ptr<Logger>(new Logger(spdLogger))));
  }
  return LoggerMap[loggerName];
}

F_STATIC_BEGIN(Log)

#ifndef NDEBUG
spdlog::set_level(spdlog::level::debug);
#else
spdlog::set_level(spdlog::level::err);
#endif

spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [thread %t] %v");
F_STATIC_END(Log)

} // namespace fastype
