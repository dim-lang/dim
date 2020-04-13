// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#include "NameGenerator.h"
#include "Counter.h"

static Counter NameGeneratorCounter;

namespace detail {

long long nameCount() { return NameGeneratorCounter.count(); }

} // namespace detail
