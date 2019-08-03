// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Profile.h"
#include <ctime>

namespace fastype {

#ifdef NDEBUG

Timer::Timer() : timestamp_(0), count_(0), stop_(false) {}

int Timer::elapse() { return count_; }

void Timer::stop() {}

void Timer::resume() {}

#else

Timer::Timer() : timestamp_(clock()), count_(0), stop_(false) {}

int Timer::elapse() { return count_; }

void Timer::stop() {
  F_CHECKF(!stop_, "stop_ {} is false", stop_);
  clock_t tmp = clock();
  count_ += tmp - timestamp_;
  timestamp_ = tmp;
  stop_ = true;
}

void Timer::resume() {
  F_CHECKF(stop_, "stop_ {} is true", stop_);
  stop_ = false;
}

#endif

} // namespace fastype
