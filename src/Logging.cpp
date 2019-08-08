// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "ConcurrentHashMap.h"
#include "StaticBlock.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "fmt/format.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
using std::make_pair;
using std::shared_ptr;
using std::string;
using std::to_string;

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

string FormatLocation(const detail::Location &location, const char *fmtMsg) {
  string shortFileName(location.fileName());
  size_t slashPos = shortFileName.find_last_of("/");
  slashPos =
      slashPos != string::npos ? slashPos : shortFileName.find_last_of("\\");
  if (slashPos != string::npos) {
    shortFileName = shortFileName.substr(slashPos + 1);
  }
  return fmt::format("[{}:{} {}] {}", shortFileName, location.lineNumber(),
                     location.functionName(), fmtMsg);
}

}; // namespace detail

static ConcurrentHashMap<string, shared_ptr<Logger>> LoggerMap;
static string FileName;

shared_ptr<Logger> LogManager::getLogger(const string &loggerName) {
  LoggerMap.lock();
  if (LoggerMap.find(loggerName) == LoggerMap.end()) {
    shared_ptr<spdlog::logger> spdlogger =
        spdlog::basic_logger_mt(loggerName, FileName);
    LoggerMap.insert(
        make_pair(loggerName, shared_ptr<Logger>(new Logger(spdlogger))));
  }
  LoggerMap.unlock();
  return LoggerMap[loggerName];
}

F_STATIC_BLOCK_BEG(Log)

#ifdef NDEBUG
spdlog::set_level(spdlog::level::err);
#else
spdlog::set_level(spdlog::level::debug);
#endif
spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e [%l] [%n] process-%P thread-%t %v");

boost::posix_time::ptime pNow = boost::posix_time::second_clock::local_time();
FileName = fmt::format(
    "fastype-{:04d}-{:02d}-{:02d}-{:02d}-{:02d}-{:02d}.log", pNow.date().year(),
    pNow.date().month(), pNow.date().day(), pNow.time_of_day().hours(),
    pNow.time_of_day().minutes(), pNow.time_of_day().seconds());

F_STATIC_BLOCK_END(Log)

} // namespace fastype
