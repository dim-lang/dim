// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Clock.h"
#include "Log.h"
#include "config/Header.h"

Clock::Clock()
    : timestamp_(std::chrono::system_clock::now()),
      count_(std::chrono::milliseconds::zero()), stop_(false) {}

int Clock::elapse() {
  std::chrono::system_clock::time_point tmp = std::chrono::system_clock::now();
  count_ +=
      std::chrono::duration_cast<std::chrono::milliseconds>(tmp - timestamp_);
  return count_.count();
}

void Clock::stop() {
  LOG_CHECK(!stop_, "stop_ {} is false", stop_);
  std::chrono::system_clock::time_point tmp = std::chrono::system_clock::now();
  count_ +=
      std::chrono::duration_cast<std::chrono::milliseconds>(tmp - timestamp_);
  timestamp_ = tmp;
  stop_ = true;
}

void Clock::resume() {
  LOG_CHECK(stop_, "stop_ {} is true", stop_);
  stop_ = false;
  timestamp_ = std::chrono::system_clock::now();
}
