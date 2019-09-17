// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Random.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>

#define TEST_MAX 16384

#define assertValid(i, IntType)                                                \
  do {                                                                         \
    IntType a = fastype::Random::next<IntType>();                              \
    IntType b = fastype::Random::next<IntType>(i + 1);                         \
    IntType c =                                                                \
        fastype::Random::next<IntType>(std::min(a, b), std::max(a, b));        \
    REQUIRE(a >= 0);                                                           \
    REQUIRE(a < std::numeric_limits<IntType>::max());                          \
    REQUIRE(b >= 0);                                                           \
    REQUIRE(b <= i);                                                           \
    REQUIRE(c >= std::min(a, b));                                              \
    REQUIRE(c < std::max(a, b));                                               \
  } while (0)

TEST_CASE("Random", "[Random]") {
  SECTION("int") {
    INFO("int");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, int);
    }
  }

  SECTION("unsigned int") {
    INFO("unsigned int");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, unsigned int);
    }
  }

  SECTION("long") {
    INFO("long");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, long);
    }
  }

  SECTION("unsigned long") {
    INFO("unsigned long");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, unsigned long);
    }
  }

  SECTION("long long") {
    INFO("long long");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, long long);
    }
  }

  SECTION("unsigned long long") {
    INFO("unsigned long long");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, unsigned long long);
    }
  }
}
