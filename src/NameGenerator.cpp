// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "NameGenerator.h"

CountNameGenerator::CountNameGenerator(const Cowstr &a_delimiter,
                                       const Cowstr &a_prefix)
    : nameGenerator_(a_delimiter, a_prefix) {}

TagNameGenerator::TagNameGenerator(const Cowstr &a_delimiter,
                                   const Cowstr &a_prefix)
    : nameGenerator_(a_delimiter, a_prefix) {}
