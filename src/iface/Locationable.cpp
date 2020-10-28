// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "iface/Locationable.h"

Locationable::Locationable(const Location &location)
    : locationable_(location) {}

Location &Locationable::location() { return locationable_; }

const Location &Locationable::location() const { return locationable_; }
