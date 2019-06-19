// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "spdlog/spdlog.h"
#include <memory>
#include <stdio.h>
#include <string>

namespace fastype {

class LoggerManager {
public:
  virtual ~LoggerManager() = default;

  static std::shared_ptr<spdlog::logger>
  getLogger(const std::string &loggerName);
};

} // namespace fastype
