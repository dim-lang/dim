// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Typeable.h"

TypeableImpl::TypeableImpl(std::shared_ptr<TypeSymbol> typeableImpl)
    : typeableImpl_(typeableImpl) {}

std::shared_ptr<TypeSymbol> TypeableImpl::type() const { return typeableImpl_; }
