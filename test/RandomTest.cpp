// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Random.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>

#define TEST_MAX 16384

#define assertValid(i, IntType)                                                \
  do {                                                                         \
    IntType a = fastype::Random::next<IntType>();                              \
    IntType b = fastype::Random::next<IntType>(                                \
        std::min<IntType>(i + 1, std::numeric_limits<IntType>::max()));        \
    IntType c =                                                                \
        fastype::Random::next<IntType>(std::min(a, b), std::max(a, b));        \
    F_INFOF("a:{} b:{} c:{}", a, b, c);                                        \
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

  SECTION("int8_t") {
    INFO("int8_t");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, int8_t);
    }
  }

  SECTION("uint8_t") {
    INFO("uint8_t");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, uint8_t);
    }
  }

  SECTION("int16_t") {
    INFO("int16_t");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, int16_t);
    }
  }

  SECTION("uint16_t") {
    INFO("uint16_t");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, uint16_t);
    }
  }

  SECTION("int32_t") {
    INFO("int32_t");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, int32_t);
    }
  }

  SECTION("uint32_t") {
    INFO("uint32_t");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, uint32_t);
    }
  }

  SECTION("int64_t") {
    INFO("int64_t");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, int64_t);
    }
  }

  SECTION("uint64_t") {
    INFO("uint64_t");
    for (int i = 0; i < TEST_MAX; i++) {
      assertValid(i, uint64_t);
    }
  }
}
