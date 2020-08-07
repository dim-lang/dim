// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Counter.h"

Counter::Counter(const unsigned long long &value) : value_(value) {}

unsigned long long Counter::count() { return value_++; }
