// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#include "Counter.h"
#include <sstream>
extern Counter NameGeneratorCounter;

template <typename A> std::string namegen(const A &a) {
  std::stringstream ss;
  ss << a << "_" << NameGeneratorCounter.count();
  return ss.str();
}

template <typename A, typename B> std::string namegen2(const A &a, const B &b) {
  std::stringstream ss;
  ss << a << "_" << b << "_" << NameGeneratorCounter.count();
  return ss.str();
}

template <typename A, typename B, typename C>
std::string namegen3(const A &a, const B &b, const C &c) {
  std::stringstream ss;
  ss << a << "_" << b << "_" << c << "_" << NameGeneratorCounter.count();
  return ss.str();
}
