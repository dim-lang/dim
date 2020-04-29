// Copyright 2019- <rooster-lang>
// Apache License Version 2.0

#include "NameGenerator.h"
#include "Counter.h"

namespace detail {

static Counter NameGeneratorCounter;

long long nameCount() { return NameGeneratorCounter.count(); }

} // namespace detail
