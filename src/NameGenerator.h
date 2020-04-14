// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#pragma once
#include "Counter.h"
#include <sstream>
#include <string>

namespace detail {

long long nameCount();

template <typename T> std::string nameGenImpl(T t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}

template <typename T, typename... R> std::string nameGenImpl(T t, R... r) {
  std::stringstream ss;
  ss << t << "_" << nameGenImpl(r...);
  return ss.str();
}

} // namespace detail

template <typename... T> std::string nameGen(T... t) {
  std::stringstream ss;
  ss << detail::nameGenImpl(t...) << "_" << detail::nameCount();
  return ss.str();
}
