// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "catch2/catch.hpp"

TEST_CASE("Line", "[Line]") {

  SECTION("Args Constructor") {
    fastype::Line l1(100, 3, true);
    REQUIRE((void *)l1.data() != nullptr);
    REQUIRE(l1.size() == 0);
    REQUIRE(l1.capacity() >= 100);
    REQUIRE(l1.dirty() == true);
    REQUIRE(l1.lineNumber() == 3);

    fastype::Line l2(490, 17, false);
    REQUIRE((void *)l2.data() != nullptr);
    REQUIRE(l2.size() == 0);
    REQUIRE(l2.capacity() >= 0);
    REQUIRE(l2.dirty() == false);
    REQUIRE(l2.lineNumber() == 17);
  }
}
