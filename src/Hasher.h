// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>

namespace fastype {

class Hasher {
public:
  static uint32_t hash32(const void *key, const int len, uint32_t seed = 0UL);
  static uint64_t hash64(const void *key, const int len, uint64_t seed = 0ULL);
};

class Ast;

} // namespace fastype

namespace std {

template <> struct hash<icu::UnicodeString> {
public:
  std::size_t operator()(const icu::UnicodeString &s) const {
    return (size_t)s.hashCode();
  }
};

template <> class hash<std::shared_ptr<fastype::Ast>> {
public:
  size_t operator()(const std::shared_ptr<fastype::Ast> &s) const {
    return (size_t)s.get();
  }
};

} // namespace std
