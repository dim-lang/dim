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

template <typename... Args> std::string nameGen(Args... args) {
  std::stringstream ss;
  ss << detail::nameGenImpl(args...) << "_" << detail::nameCount();
  return ss.str();
}

template <typename V, typename... Args>
std::string nameGenWith(V v, Args... args) {
  std::stringstream ss;
  ss << v << "@" << detail::nameGenImpl(args...) << "_" << detail::nameCount();
  return ss.str();
}
