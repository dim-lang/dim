// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <algorithm>
#include <cstdint>
#include <cstdlib>

class Hash {
public:
  static uint32_t hash32(const void *key, const int len, uint32_t seed = 0UL);
  static uint64_t hash64(const void *key, const int len, uint64_t seed = 0ULL);
};
