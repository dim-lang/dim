// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "NameGenerator.h"

NameGenerator::NameGenerator(const std::string &prefix,
                             const std::string &delimiter,
                             const std::string &withDelimiter)
    : prefix_(prefix), delimiter_(delimiter), withDelimiter_(withDelimiter) {}
