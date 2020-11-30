// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "iface/Identifiable.h"
#include "fmt/format.h"
#include "infra/Counter.h"
#include "infra/Log.h"
#include <string>

static Counter IdentifyCounter;

Identifiable::Identifiable() : identifiable_(IdentifyCounter.count()) {}

unsigned long long Identifiable::identifier() const { return identifiable_; }

Cowstr Identifiable::hexIdentifier() const {
  return fmt::format("{0:x}", identifiable_);
}

Cowstr Identifiable::decIdentifier() const {
  return fmt::format("{}", identifiable_);
}

Cowstr Identifiable::octIdentifier() const {
  return fmt::format("{0:o}", identifiable_);
}

Cowstr Identifiable::binIdentifier() const {
  return fmt::format("{0:b}", identifiable_);
}
