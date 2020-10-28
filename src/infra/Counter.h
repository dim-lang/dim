// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "infra/Cowstr.h"

class Counter {
public:
  Counter(unsigned long long value = 1ULL);
  virtual ~Counter() = default;
  unsigned long long count();
  unsigned long long total() const;

private:
  unsigned long long value_;
};
