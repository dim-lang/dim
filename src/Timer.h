// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <chrono>
#include <string>

namespace fastype {

class Timer {
public:
  // auto start in constructor
  Timer();
  Timer(const Timer &) = default;
  Timer &operator=(const Timer &) = default;
  Timer(Timer &&) = default;
  Timer &operator=(Timer &&) = default;
  virtual ~Timer() = default;
  int elapse();
  void stop();
  void resume();

private:
  std::chrono::system_clock::time_point timestamp_;
  std::chrono::milliseconds count_;
  bool stop_;
};

} // namespace fastype
