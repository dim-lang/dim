// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <chrono>
#include <string>

class Clock {
public:
  // auto start in constructor
  Clock();
  Clock(const Clock &) = default;
  Clock &operator=(const Clock &) = default;
  Clock(Clock &&) = default;
  Clock &operator=(Clock &&) = default;
  virtual ~Clock() = default;
  int elapse();
  void stop();
  void resume();

private:
  std::chrono::system_clock::time_point timestamp_;
  std::chrono::milliseconds count_;
  bool stop_;
};
