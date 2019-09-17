// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Block.h"
#include "Random.h"
#include "catch2/catch.hpp"
#include <cstring>

#define TEST_MAX 128

TEST_CASE("Block", "[Block]") {

  SECTION("Empty Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      fastype::Block b;
      REQUIRE(b.capacity() == 0);
      REQUIRE(b.empty());
      REQUIRE(b.size() == 0);
      REQUIRE(b.head() == nullptr);
      REQUIRE(b.tail() == nullptr);
    }
  }

  SECTION("Capacity Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      int r = fastype::Random::next<int>(TEST_MAX);
      fastype::Block b(r);
      REQUIRE(b.capacity() == r);
      REQUIRE(b.empty());
      REQUIRE(b.size() == 0);
      REQUIRE(b.head() == nullptr);
      REQUIRE(b.tail() == nullptr);
    }
  }
}
