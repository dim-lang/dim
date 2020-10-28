// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Location.h"
#include "catch2/catch.hpp"

TEST_CASE("Location", "[Location]") {
  SECTION("Location") {
    Location loc;
    REQUIRE(loc.begin == loc.end);
  }
}
