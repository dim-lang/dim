// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

class Counter {
public:
  Counter(unsigned long long value = 1ULL);
  virtual ~Counter() = default;
  unsigned long long next();
  unsigned long long count() const;

private:
  unsigned long long value_;
};
