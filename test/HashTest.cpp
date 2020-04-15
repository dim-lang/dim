// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "Hash.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Hash", "[Hash]") {
  int a1 = 1, a2 = 2;
  long long b1 = 1LL, b2 = 2LL;

  SECTION("hash") {
    LOG_INFO("hash32 key: {}, value: {}", a1, Hash::hash32(&a1, sizeof(a1)));
    LOG_INFO("hash32 key: {}, value: {}", a2, Hash::hash32(&a2, sizeof(a2)));
    LOG_INFO("hash32 key: {}, value: {}", b1, Hash::hash32(&b1, sizeof(b1)));
    LOG_INFO("hash32 key: {}, value: {}", b2, Hash::hash32(&b2, sizeof(b2)));

    LOG_INFO("hash64 key: {}, value: {}", a1, Hash::hash64(&a1, sizeof(a1)));
    LOG_INFO("hash64 key: {}, value: {}", a2, Hash::hash64(&a2, sizeof(a2)));
    LOG_INFO("hash64 key: {}, value: {}", b1, Hash::hash64(&b1, sizeof(b1)));
    LOG_INFO("hash64 key: {}, value: {}", b2, Hash::hash64(&b2, sizeof(b2)));
  }
}
