// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "system/DateTime.h"
#include <chrono>
#include <cstdint>

namespace fastype {

int64_t DateTime::now() {
  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::system_clock::now();
  auto dura = now.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::seconds>(dura).count();
}

int64_t DateTime::millinow() {
  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::system_clock::now();
  auto dura = now.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(dura).count();
}

}; // namespace fastype

} // namespace fastype
