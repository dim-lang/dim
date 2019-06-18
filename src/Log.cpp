// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include <memory>

namespace fastype {

std::shared_ptr<Logger>
LoggerManager::getLogger(const std::string &loggerName) {
  return std::make_shared<Logger>();
}

} // namespace fastype

