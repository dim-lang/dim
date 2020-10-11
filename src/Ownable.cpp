// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ownable.h"

OwnableImpl::OwnableImpl(std::shared_ptr<Scope> ownableImpl)
    : ownableImpl_(ownableImpl) {}

std::shared_ptr<Scope> OwnableImpl::owner() const { return ownableImpl_; }
