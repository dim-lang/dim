// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Approximate.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <cstring>

TEST_CASE("Approximate", "[Approximate]") {
  SECTION("float double") {
    REQUIRE(!fastype::Approximate::eq((float)0.0001F, (float)0.0009F));
    REQUIRE(fastype::Approximate::eq((float)0.00013F, (float)0.00013F));
    REQUIRE(fastype::Approximate::eq((float)0.000009F, (float)0.00001F));
  }
}
