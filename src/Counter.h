// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once

class Counter {
public:
  Counter(const long long &value = 1LL);
  virtual ~Counter() = default;
  virtual long long count();

private:
  long long value_;
};
