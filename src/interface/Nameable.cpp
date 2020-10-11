// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Nameable.h"

NameableImpl::NameableImpl() : nameableImpl_("") {}

NameableImpl::NameableImpl(const Cowstr &name) : nameableImpl_(name) {}

Cowstr &NameableImpl::name() { return nameableImpl_; }

const Cowstr &NameableImpl::name() const { return nameableImpl_; }
