// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#pragma once
#include "Counter.h"
#include <sstream>
#include <string>

namespace detail {

long long nameCount();

} // namespace detail

template <typename T> std::string nameGen(const T &t) {
  std::stringstream ss;
  ss << t << "_" << detail::nameCount();
  return ss.str();
}

template <typename V, typename T> std::string nameGen(const V &v, const T &t) {
  std::stringstream ss;
  ss << v << "@" << t << "_" << detail::nameCount();
  return ss.str();
}
