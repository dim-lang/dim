// Copyright 2019- <fastype.org>

#include <memory>
#include <stdio.h>

namespace fastype {

class Logger {
public:
  void info() { printf("info\n"); }
  void debug() { printf("debug\n"); }
};

class LoggerManager {
public:
  static std::shared_ptr<Logger> getLogger(const std::string &loggerName);
};

} // namespace fastype
