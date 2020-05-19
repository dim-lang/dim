// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "NameGenerator.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("NameGenerator", "[NameGenerator]") {
  SECTION("nameGen") {
    NameGenerator generator;
    REQUIRE(generator.generate("a").substr(0, 2) == std::string("a_"));
    REQUIRE(generator.generate("a", "b").substr(0, 4) == std::string("a_b_"));
    REQUIRE(generator.generate("a", "b", 1, 2.3).substr(0, 10) ==
            std::string("a_b_1_2.3_"));
    REQUIRE(generator.generateWith("c", "d").substr(0, 4) ==
            std::string("c@d_"));
    REQUIRE(generator.generateWith("c", "d", 4, 5.6).substr(0, 10) ==
            std::string("c@d_4_5.6_"));
  }
}
