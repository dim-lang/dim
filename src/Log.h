// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include <memory>
#include <stdio.h>
#include <string>

namespace fastype {

class Logger {
public:
  virtual ~Logger() = default;

  virtual void info() = 0;
  virtual void debug() = 0;
};

class LoggerManager {
public:
  virtual ~LoggerManager() = default;

  static std::shared_ptr<Logger> getLogger(const std::string &loggerName);
};

} // namespace fastype
