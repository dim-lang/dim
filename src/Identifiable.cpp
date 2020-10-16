// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Identifiable.h"
#include "Counter.h"

Identifiable::Identifiable()
    : identifiable_(Counter::instance("Identifiable")->count()) {}

unsigned long long Identifiable::identifier() const { return identifiable_; }
