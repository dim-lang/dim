// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Name.h"

Nameable::Nameable() : name_("") {}

Nameable::Nameable(const Cowstr &name) : name_(name) {}

Cowstr &Nameable::name() { return name_; }

const Cowstr &Nameable::name() const { return name_; }
