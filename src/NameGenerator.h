// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Counter.h"
#include <sstream>
#include <string>

namespace detail {

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

class NameGenerator {
public:
  template <typename... Args> std::string generate(Args... args) {
    std::stringstream ss;
    ss << detail::nameGenImpl(args...) << "_" << counter_.count();
    return ss.str();
  }

  template <typename V, typename... Args>
  std::string generateWith(V v, Args... args) {
    std::stringstream ss;
    ss << v << "@" << detail::nameGenImpl(args...) << "_" << counter_.count();
    return ss.str();
  }

private:
  Counter counter_;
};
