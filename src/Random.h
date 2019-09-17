// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include <cmath>
#include <cstdint>
#include <limits>
#include <random>
#include <string>

namespace fastype {

class Random {
public:
  // @return   [0, limit)
  template <typename T> static T next(T limit = std::numeric_limits<T>::max()) {
    F_CHECKF(limit > 0, "limit {} > 0", limit);
    return next<T>(0, limit);
  }

  // @return   [start, limit)
  template <typename T> static T next(T start, T limit) {
    return (llong_(engine_) % (limit - start)) + start;
  }

  static std::string nextAlpha(int limit);

  static std::string nextAlphaNumeric(int limit);

  static std::string nextDigit(int limit);

  static std::string nextString(int limit, const char *candidates, int c);

  static std::string nextString(int limit, const std::string &candidates);

private:
  static std::random_device device_;
  static std::mt19937 engine_;
  static std::uniform_int_distribution<long long> llong_;
};

} // namespace fastype
