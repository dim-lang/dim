// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Counter.h"

Counter::Counter(const long long &value) : value_(value) {}

long long Counter::count() { return value_++; }
