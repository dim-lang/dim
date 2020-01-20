// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>

class Hasher {
public:
  static uint32_t hash32(const void *key, const int len, uint32_t seed = 0UL);
  static uint64_t hash64(const void *key, const int len, uint64_t seed = 0ULL);
};

namespace dsl {

class Ast;

}

namespace std {

template <> struct hash<icu::UnicodeString> {
public:
  std::size_t operator()(const icu::UnicodeString &s) const {
    return (size_t)s.hashCode();
  }
};

template <> class hash<std::shared_ptr<dsl::Ast>> {
public:
  size_t operator()(const std::shared_ptr<dsl::Ast> &s) const {
    return (size_t)s.get();
  }
};

} // namespace std
