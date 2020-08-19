// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "DateTime.h"
#include <chrono>
#include <cstdint>

long long DateTime::now() {
  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::system_clock::now();
  auto dura = now.time_since_epoch();
  return (long long)std::chrono::duration_cast<std::chrono::seconds>(dura)
      .count();
}

long long DateTime::nowms() {
  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::system_clock::now();
  auto dura = now.time_since_epoch();
  return (long long)std::chrono::duration_cast<std::chrono::milliseconds>(dura)
      .count();
}
