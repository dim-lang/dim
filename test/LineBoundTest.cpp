// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "LineBound.h"
#include "catch2/catch.hpp"

TEST_CASE("Line Bound", "[LineBound]") {
  INFO("Line Bound");

  fastype::LineBound a;
  fastype::LineBound b(1, 2);
  REQUIRE(a.buffer() == -1);
  REQUIRE(a.byte() == -1);
  REQUIRE(b.buffer() == 1);
  REQUIRE(b.byte() == 2);
}
