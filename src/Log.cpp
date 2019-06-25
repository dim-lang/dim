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
using std::lock_guard;
using std::make_pair;
using std::mutex;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::unordered_map;

namespace fastype {

string Logger::formatLocation(const detail::LogLocation &location,
                              const char *fmt) {
  string shortFileName(location.fileName);
  size_t slashPos = shortFileName.find_last_of("/");
  slashPos =
      slashPos != string::npos ? slashPos : shortFileName.find_last_of("\\");
  if (slashPos != string::npos) {
    shortFileName = shortFileName.substr(slashPos + 1);
  }
  return string(string("[") + shortFileName + "] [" + location.functionName +
                ":" + to_string(location.lineNumber) + "] " + fmt);
}

static mutex LoggerLock;
static mutex LogSinkLock;
static unordered_map<string, shared_ptr<Logger>> LoggerMap =
    unordered_map<string, shared_ptr<Logger>>();
static unordered_map<string, shared_ptr<detail::LogSink>> LogSinkMap =
    unordered_map<string, shared_ptr<detail::LogSink>>();
static const string FileName = "fastype.log";
static const int MaxFileSize = 1048576 * 10;
static const int MaxFiles = 100;

static shared_ptr<detail::LogSink> openSink(const string &sinkName) {
  lock_guard<mutex> guard(LogSinkLock);
  if (LogSinkMap.find(sinkName) == LogSinkMap.end()) {
    shared_ptr<detail::LogSink> sink =
        shared_ptr<detail::LogSink>(new detail::LogSink(sinkName));
    LogSinkMap.insert(make_pair(sinkName, sink));
  }
  return LogSinkMap[sinkName];
}

shared_ptr<Logger> LogManager::getLogger(const string &loggerName) {
  lock_guard<mutex> guard(LoggerLock);
  if (LoggerMap.find(loggerName) == LoggerMap.end()) {
    shared_ptr<Logger> logger =
        shared_ptr<Logger>(new Logger(openSink(FileName)));
    LoggerMap.insert(make_pair(loggerName, logger));
  }
  return LoggerMap[loggerName];
}

F_STATIC_BLOCK_BEGIN(Log)

#ifndef NDEBUG
spdlog::set_level(spdlog::level::debug);
#else
spdlog::set_level(spdlog::level::err);
#endif

spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [thread %t] %v");
F_STATIC_BLOCK_END(Log)

} // namespace fastype
