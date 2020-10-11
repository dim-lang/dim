// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "interface/Identifiable.h"
#include "Counter.h"

static Counter IdentifiableImplCounter;

IdentifiableImpl::IdentifiableImpl()
    : identifiableImpl_(IdentifiableImplCounter.next()) {}

IdentifiableImpl::IdentifiableImpl(unsigned long long identifiableImpl)
    : identifiableImpl_(identifiableImpl) {}

unsigned long long IdentifiableImpl::identifier() const {
  return identifiableImpl_;
}
