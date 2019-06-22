// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Util.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <time.h>
#include <chrono>

namespace fastype {

uint64_t nowTimestamp() {
  auto now = std::chrono::system_clock::now();
  return (uint64_t)std::chrono::system_clock::to_time_t(now);
}

std::string nowDatetime() { return ""; }

std::string formatDatetime(uint64_t t, const std::string &fmt) { return ""; }

uint64_t parseDatetime(const std::string &datetime, const std::string &fmt) {
  return 0;
}

} // namespace fastype
