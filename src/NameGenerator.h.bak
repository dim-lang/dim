// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Counter.h"
#include <sstream>
#include <string>

namespace detail {

struct NameGeneratorImpl {
  NameGeneratorImpl(const std::string &delimiter) : delimiter_(delimiter) {}
  virtual ~NameGeneratorImpl() = default;

  template <typename T> std::string generateImpl(T t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
  }

  template <typename T, typename... R> std::string generateImpl(T t, R... r) {
    std::stringstream ss;
    ss << t << delimiter_ << generateImpl(r...);
    return ss.str();
  }

private:
  std::string delimiter_;
};

} // namespace detail

class NameGenerator {
public:
  NameGenerator(const std::string &prefix, const std::string &delimiter,
                const std::string &withDelimiter);
  virtual ~NameGenerator() = default;

  template <typename... Args> std::string generate(Args... args) {
    std::stringstream ss;
    detail::NameGeneratorImpl namegen(delimiter_);
    ss << prefix_ << namegen.generateImpl(args...) << delimiter_
       << counter_.count();
    return ss.str();
  }

  template <typename V, typename... Args>
  std::string generateWith(V with, Args... args) {
    std::stringstream ss;
    detail::NameGeneratorImpl namegen(delimiter_);
    ss << prefix_ << with << withDelimiter_ << namegen.generateImpl(args...)
       << delimiter_ << counter_.count();
    return ss.str();
  }

private:
  Counter counter_;
  std::string prefix_;
  std::string delimiter_;
  std::string withDelimiter_;
};
