// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Name.h"

NameableImpl::NameableImpl() : nameableImpl_("") {}

NameableImpl::NameableImpl(const Cowstr &name) : nameableImpl_(name) {}

Cowstr &NameableImpl::name() { return nameableImpl_; }

const Cowstr &NameableImpl::name() const { return nameableImpl_; }

NameGenerator::NameGenerator(const Cowstr &a_delimiter, const Cowstr &a_prefix,
                             const Cowstr &a_postfix)
    : delimiter(a_delimiter), prefix(a_prefix), postfix(a_postfix) {}

CounterNameGenerator::CounterNameGenerator(const Cowstr &a_delimiter,
                                           const Cowstr &a_prefix)
    : nameGenerator_(a_delimiter, a_prefix) {}

TagNameGenerator::TagNameGenerator(const Cowstr &a_delimiter,
                                   const Cowstr &a_prefix)
    : nameGenerator_(a_delimiter, a_prefix) {}
