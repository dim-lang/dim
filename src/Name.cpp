// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Name.h"

Counter CounterNameGenerator::CoutnerNameGeneratorCounter_;

Nameable::Nameable(const Cowstr &name) : nameable_(name) {}

Cowstr &Nameable::name() { return nameable_; }

const Cowstr &Nameable::name() const { return nameable_; }

NameGenerator::NameGenerator(const Cowstr &a_delimiter, const Cowstr &a_prefix,
                             const Cowstr &a_postfix)
    : delimiter(a_delimiter), prefix(a_prefix), postfix(a_postfix) {}

CounterNameGenerator::CounterNameGenerator(const Cowstr &a_delimiter,
                                           const Cowstr &a_prefix)
    : nameGenerator_(a_delimiter, a_prefix) {}

TagNameGenerator::TagNameGenerator(const Cowstr &a_delimiter,
                                   const Cowstr &a_prefix)
    : nameGenerator_(a_delimiter, a_prefix) {}
