// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "NameGenerator.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("NameGenerator", "[NameGenerator]") {
  SECTION("nameGen") {
    REQUIRE(nameGen("a").substr(0, 2) == std::string("a_"));
    REQUIRE(nameGen("a", "b").substr(0, 4) == std::string("a_b_"));
    REQUIRE(nameGen("a", "b", 1, 2.3).substr(0, 10) ==
            std::string("a_b_1_2.3_"));
    REQUIRE(nameGenWith("c", "d").substr(0, 4) == std::string("c@d_"));
    REQUIRE(nameGenWith("c", "d", 4, 5.6).substr(0, 10) ==
            std::string("c@d_4_5.6_"));
  }
}
