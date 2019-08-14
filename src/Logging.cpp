// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
using std::make_pair;
using std::shared_ptr;
using std::string;
using std::to_string;

namespace fastype {

void Logging::initialize(const std::string &fileName) {

#ifdef NDEBUG
  spdlog::set_level(spdlog::level::err);
#else
  spdlog::set_level(spdlog::level::debug);
#endif
  spdlog::set_pattern(
      "%Y-%m-%d %H:%M:%S.%e [%n] %l process-%P thread-%t [%@ %!] %v");

  boost::posix_time::ptime pNow = boost::posix_time::second_clock::local_time();
  string fullFileName =
      fmt::format("{}-{:04d}-{:02d}-{:02d}-{:02d}-{:02d}-{:02d}.log", fileName,
                  pNow.date().year(), pNow.date().month(), pNow.date().day(),
                  pNow.time_of_day().hours(), pNow.time_of_day().minutes(),
                  pNow.time_of_day().seconds());
  shared_ptr<spdlog::logger> defaultLogger =
      spdlog::basic_logger_mt(fileName, fullFileName);
  spdlog::set_default_logger(defaultLogger);
}

} // namespace fastype
