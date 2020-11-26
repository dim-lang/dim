// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "NameGenerator.h"

Counter CounterNameGenerator::counter_;

NameGenerator::NameGenerator(const Cowstr &a_delimiter, const Cowstr &a_prefix,
                             const Cowstr &a_postfix)
    : delimiter(a_delimiter), prefix(a_prefix), postfix(a_postfix) {}

CounterNameGenerator::CounterNameGenerator(const Cowstr &a_delimiter,
                                           const Cowstr &a_prefix)
    : nameGenerator_(a_delimiter, a_prefix) {}

TagNameGenerator::TagNameGenerator(const Cowstr &a_delimiter,
                                   const Cowstr &a_prefix)
    : nameGenerator_(a_delimiter, a_prefix) {}
