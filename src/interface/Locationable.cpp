// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "interface/Locationable.h"

LocationableImpl::LocationableImpl(const Location &location)
    : locationableImpl_(location) {}

Location &LocationableImpl::location() { return locationableImpl_; }

const Location &LocationableImpl::location() const { return locationableImpl_; }
