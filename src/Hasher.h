// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstdint>
#include <cstdlib>

namespace fastype {

class Hasher {
public:
  uint32_t hash32(const void *key, const int len, uint32_t seed = 0UL);
  uint64_t hash64(const void *key, const int len, uint64_t seed = 0ULL);
};

} // namespace fastype
