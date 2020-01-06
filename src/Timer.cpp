// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Timer.h"
#include "Logging.h"
#include "config/Header.h"

Timer::Timer()
    : timestamp_(std::chrono::system_clock::now()),
      count_(std::chrono::milliseconds::zero()), stop_(false) {}

int Timer::elapse() {
  std::chrono::system_clock::time_point tmp = std::chrono::system_clock::now();
  count_ +=
      std::chrono::duration_cast<std::chrono::milliseconds>(tmp - timestamp_);
  return count_.count();
}

void Timer::stop() {
  FCHECK(!stop_, "stop_ {} is false", stop_);
  std::chrono::system_clock::time_point tmp = std::chrono::system_clock::now();
  count_ +=
      std::chrono::duration_cast<std::chrono::milliseconds>(tmp - timestamp_);
  timestamp_ = tmp;
  stop_ = true;
}

void Timer::resume() {
  FCHECK(stop_, "stop_ {} is true", stop_);
  stop_ = false;
  timestamp_ = std::chrono::system_clock::now();
}
