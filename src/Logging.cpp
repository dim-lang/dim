// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <string>

namespace fastype {

void Logging::initialize(const std::string &logPath,
                         const std::string &logName) {

#ifdef NDEBUG
  spdlog::set_level(spdlog::level::err);
#else
  spdlog::set_level(spdlog::level::info);
#endif

  spdlog::set_pattern(
      "%Y-%m-%d %H:%M:%S.%e [%n] %l process-%P thread-%t [%@ %!] %v");

  boost::posix_time::ptime pNow = boost::posix_time::second_clock::local_time();
  std::string fullFileName =
      fmt::format("{}/{}-{:04d}-{:02d}-{:02d}-{:02d}-{:02d}-{:02d}.log",
                  logPath, logName, pNow.date().year(), pNow.date().month(),
                  pNow.date().day(), pNow.time_of_day().hours(),
                  pNow.time_of_day().minutes(), pNow.time_of_day().seconds());
  std::shared_ptr<spdlog::logger> defaultLogger =
      spdlog::basic_logger_mt(logName, fullFileName);
  spdlog::set_default_logger(defaultLogger);
}

} // namespace fastype
