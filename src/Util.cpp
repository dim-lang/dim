// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Util.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>

namespace fastype {

int64_t nowTimestamp() {
  time_t t =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  return (int64_t)t;
}

std::string nowDateTime() { return formatDatetime(nowTimestamp()); }

std::string formatDatetime(int64_t t, const char *fmt) {
  std::tm tm;
  localtime_s(&t, &tm);
  std::ostringstream oss;
  oss << std::put_time(&tm, fmt);
  return oss.str();
}

std::string formatDatetime(int64_t t) {
  return formatDatetime(t, "%Y-%m-%d %H:%M:%S");
}

} // namespace fastype
