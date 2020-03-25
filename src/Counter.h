// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#pragma once
#include <atomic>

class Counter {
public:
  Counter(const long long &value = 1LL);
  virtual ~Counter() = default;
  virtual long long count();

private:
  std::atomic_llong value_;
};
