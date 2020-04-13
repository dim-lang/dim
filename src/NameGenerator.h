// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#pragma once
#include "Counter.h"
#include <sstream>
#include <string>

namespace detail {

long long nameCount();

}

template <typename A> std::string namegen(const A &a) {
  std::stringstream ss;
  ss << a << "_" << detail::nameCount();
  return ss.str();
}

template <typename A, typename B> std::string namegen2(const A &a, const B &b) {
  std::stringstream ss;
  ss << a << "_" << b << "_" << detail::nameCount();
  return ss.str();
}

template <typename A, typename B, typename C>
std::string namegen3(const A &a, const B &b, const C &c) {
  std::stringstream ss;
  ss << a << "_" << b << "_" << c << "_" << detail::nameCount();
  return ss.str();
}
