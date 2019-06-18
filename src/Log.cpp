#include "Log.h"
#include <memory>

namespace fastype {

std::shared_ptr<Logger>
LoggerManager::getLogger(const std::string &loggerName) {
  return std::make_shared<Logger>();
}

} // namespace fastype
