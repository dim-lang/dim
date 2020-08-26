// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Locationly.h"

Locationly::Locationly(const yy::location &location) : location_(location) {}

yy::location &Locationly::location() { return location_; }

const yy::location &Locationly::location() const { return location_; }
