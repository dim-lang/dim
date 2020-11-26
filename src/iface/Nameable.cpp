// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "iface/Nameable.h"

Nameable::Nameable(const Cowstr &name) : nameable_(name) {}

Cowstr &Nameable::name() { return nameable_; }

const Cowstr &Nameable::name() const { return nameable_; }
