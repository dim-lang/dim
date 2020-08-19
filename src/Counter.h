// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <atomic>

class Counter {
public:
  Counter(unsigned long long value = 1ULL);
  virtual ~Counter() = default;
  virtual unsigned long long next();

  static unsigned long long get();

private:
  unsigned long long value_;
};
