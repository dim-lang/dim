// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "fmt/format.h"
#include "infra/Counter.h"
#include "infra/Cowstr.h"
#include <algorithm>
#include <sstream>

class NameGenerator {
public:
  NameGenerator(const Cowstr &a_delimiter = "", const Cowstr &a_prefix = "",
                const Cowstr &a_postfix = "");
  virtual ~NameGenerator() = default;

  Cowstr delimiter;
  Cowstr prefix;
  Cowstr postfix;

  template <typename... Args> Cowstr generate(Args... args) {
    std::stringstream ss;
    if (!prefix.empty()) {
      ss << prefix << delimiter;
    }
    ss << generateImpl(args...);
    if (!postfix.empty()) {
      ss << delimiter << postfix;
    }
    return ss.str();
  }

private:
  template <typename T> Cowstr generateImpl(T a) {
    std::stringstream ss;
    ss << a;
    return ss.str();
  }
  template <typename T, typename... Args>
  Cowstr generateImpl(T a, Args... args) {
    std::stringstream ss;
    ss << a << delimiter << generateImpl(args...);
    return ss.str();
  }
};

class CounterNameGenerator {
public:
  CounterNameGenerator(const Cowstr &a_delimiter = "",
                       const Cowstr &a_prefix = "");
  virtual ~CounterNameGenerator() = default;

  template <typename... Args> Cowstr generate(Args... args) {
    std::stringstream ss;
    ss << nameGenerator_.generate(args...) << nameGenerator_.delimiter
       << counter_.count();
    return ss.str();
  }

private:
  static Counter counter_;
  NameGenerator nameGenerator_;
};

class TagNameGenerator {
public:
  TagNameGenerator(const Cowstr &a_delimiter = "", const Cowstr &a_prefix = "");
  virtual ~TagNameGenerator() = default;

  template <typename... Args> Cowstr generate(Args... args) {
    std::stringstream ss;
    Cowstr value = nameGenerator_.generate(args...);
    ss << value << nameGenerator_.delimiter
       << fmt::format("0:x", std::hash<Cowstr>()(value));
    return ss.str();
  }

private:
  friend class NameGenerator;
  NameGenerator nameGenerator_;
};
