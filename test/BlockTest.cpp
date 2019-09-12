// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Block.h"
#include "catch2/catch.hpp"
#include <cstring>

#define HELLO_WORLD "hello world"
#define GOODBYE_WORLD "goodbye world"

TEST_CASE("Block", "[Block]") {

  SECTION("Constructor") {
    fastype::Block b;
    REQUIRE(b.capacity() == 0);
    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);
    REQUIRE(b.head() == nullptr);
    REQUIRE(b.tail() == nullptr);
  }
}
