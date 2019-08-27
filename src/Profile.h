// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <boost/assert.hpp>
#include <boost/preprocessor/cat.hpp>
#include <chrono>
#include <fmt/format.h>
#include <string>

#ifdef NDEBUG

#define F_CHECK(cond, msg)
#define F_CHECKF(cond, fmtMsg, ...)

#else

#define F_CHECK(cond, msg) BOOST_ASSERT_MSG(cond, msg)
#define F_CHECKF(cond, fmtMsg, ...)                                            \
  BOOST_ASSERT_MSG(cond, fmt::format(fmtMsg, __VA_ARGS__).data())

#endif

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
