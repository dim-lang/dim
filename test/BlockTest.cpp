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
      REQUIRE(b.tail() == b.head());
    }
  }

  SECTION("Capacity Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      int r = fastype::Random::nextInt(TEST_MAX);
      fastype::Block b(r);
      REQUIRE(b.capacity() >= r);
      REQUIRE(b.empty());
      REQUIRE(b.size() == 0);
      REQUIRE(b.tail() == b.head());
    }
  }

  SECTION("Copy Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      int r = fastype::Random::nextInt(TEST_MAX);
      fastype::Block b1(r);
      REQUIRE(b1.capacity() >= r);
      REQUIRE(b1.empty());
      REQUIRE(b1.size() == 0);
      REQUIRE(b1.tail() == b1.head());

      fastype::Block b2(b1);
      REQUIRE(b2.capacity() >= r);
      REQUIRE(b2.empty());
      REQUIRE(b2.size() == 0);
      REQUIRE(b2.tail() == b2.head());

      std::string rs = fastype::Random::nextAlphaNumeric(i);
      fastype::Block b3(rs);
      fastype::Block b4(b3);

      REQUIRE(b3.size() == (int)rs.length());
      REQUIRE(b3.size() == b4.size());
      REQUIRE(b3.empty() == rs.empty());
      REQUIRE(b3.empty() == b4.empty());
      REQUIRE(std::memcmp(b3.head(), rs.data(), b3.size()) == 0);
      REQUIRE(std::memcmp(b3.head(), b4.head(), b3.size()) == 0);
    }
  }
}
