// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Counter.h"

Counter::Counter(unsigned long long value) : value_(value) {}

unsigned long long Counter::next() { return value_++; }

unsigned long long Counter::count() const { return value_; }
