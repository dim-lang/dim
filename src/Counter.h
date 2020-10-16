// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include <unordered_map>

class Counter {
public:
  Counter(unsigned long long value = 1ULL);
  virtual ~Counter() = default;
  unsigned long long count();
  unsigned long long total() const;

  static Counter *instance(const Cowstr &name);

private:
  unsigned long long value_;

  static std::unordered_map<Cowstr, Counter *> instances_;
};
