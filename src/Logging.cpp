// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "StaticBlock.h"
#include "fmt/format.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
using std::lock_guard;
using std::make_pair;
using std::mutex;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::unordered_map;

namespace fastype {

namespace detail {
Location::Location(const char *fileName, int lineNumber,
                   const char *functionName)
    : fileName_(fileName), lineNumber_(lineNumber),
      functionName_(functionName) {}

const std::string &Location::fileName() const { return fileName_; }

int Location::lineNumber() const { return lineNumber_; }

const std::string &Location::functionName() const { return functionName_; }

std::string Location::toString() const {
  return fmt::format("{}:{} {}", fileName_, lineNumber_, functionName_);
};

}; // namespace detail

static mutex LoggerLock;
static unordered_map<string, shared_ptr<Logger>> LoggerMap =
    unordered_map<string, shared_ptr<Logger>>();
static const string FileName = "fastype.log";
// static const int MaxFileSize = 1048576 * 10;
// static const int MaxFiles = 100;

string Logger::formatLocation(const detail::Location &location,
                              const char *fmt) {
  string shortFileName(location.fileName());
  size_t slashPos = shortFileName.find_last_of("/");
  slashPos =
      slashPos != string::npos ? slashPos : shortFileName.find_last_of("\\");
  if (slashPos != string::npos) {
    shortFileName = shortFileName.substr(slashPos + 1);
  }
  return fmt::format("[{}] [{}] [{}] {}", shortFileName,
                     location.functionName(), location.lineNumber(), fmt);
}

shared_ptr<Logger> LogManager::getLogger(const string &loggerName) {
  lock_guard<mutex> guard(LoggerLock);
  if (LoggerMap.find(loggerName) == LoggerMap.end()) {
    shared_ptr<spdlog::logger> spdlogger =
        spdlog::basic_logger_mt(loggerName, FileName);
    LoggerMap.insert(
        make_pair(loggerName, shared_ptr<Logger>(new Logger(spdlogger))));
  }
  return LoggerMap[loggerName];
}

F_STATIC_BLOCK_BEG(Log)

#ifdef NDEBUG
spdlog::set_level(spdlog::level::err);
#else
spdlog::set_level(spdlog::level::debug);
#endif
spdlog::set_pattern(
    "[%Y-%m-%d %H:%M:%S.%e] [process %P] [thread %t] [%n] [%l] %v");

F_STATIC_BLOCK_END(Log)

} // namespace fastype
