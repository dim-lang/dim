// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Identifiable.h"
#include "Counter.h"
#include "fmt/format.h"
#include <string>

static Counter IdentifyCounter;

Identifiable::Identifiable() : identifiable_(IdentifyCounter.count()) {}

unsigned long long Identifiable::identifier() const { return identifiable_; }

Cowstr Identifiable::hexIdentifier() const {
  return fmt::format("{0:x}", identifiable_);
}

Cowstr Identifiable::decIdentifier() const {
  return std::to_string(identifiable_);
}

Cowstr Identifiable::octIdentifier() const {
  return fmt::format("{0:o}", identifiable_);
}

Cowstr Identifiable::binIdentifier() const {
  return fmt::format("{0:b}", identifiable_);
}
