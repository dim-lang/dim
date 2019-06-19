// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "spdlog/spdlog.h"
#include <memory>
#include <stdio.h>
#include <string>

namespace fastype {

class LogLocation {
public:
  LogLocation(const char *fileName, int lineNumber, const char *functionName)
      : fileName(fileName), lineNumber(lineNumber), functionName(functionName) {
  }
  virtual ~LogLocation() = default;

  std::string fileName;
  int lineNumber;
  std::string functionName;
};

class LoggerManager {
public:
  virtual ~LoggerManager() = default;

  static std::shared_ptr<spdlog::logger>
  getLogger(const std::string &loggerName);

  void log() {
    std::stringbuf sb;
    sb.append("[");
  }
};

} // namespace fastype
