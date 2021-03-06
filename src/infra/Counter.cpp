// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "infra/Counter.h"

Counter::Counter(unsigned long long value) : value_(value) {}

unsigned long long Counter::count() { return value_++; }

unsigned long long Counter::total() const { return value_; }
