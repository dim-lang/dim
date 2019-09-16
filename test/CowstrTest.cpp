// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Cowstr.h"
#include "catch2/catch.hpp"
#include <cstring>

#define HELLO_WORLD "hello world"
#define GOODBYE_WORLD "goodbye world"

TEST_CASE("Cowstr", "[Cowstr]") {

  SECTION("Constructor") {
    fastype::Cowstr cs;
    REQUIRE(cs.capacity() == 0);
    REQUIRE(cs.empty());
    REQUIRE(cs.size() == 0);
    REQUIRE(cs.head() == nullptr);
    REQUIRE(cs.tail() == nullptr);
  }
}
