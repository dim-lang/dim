// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "NameGenerator.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("NameGenerator", "[NameGenerator]") {
  SECTION("Ast Name Generator") {
    NameGenerator astgen("", "_", "@");
    REQUIRE(astgen.generate("a").substr(0, 2) == std::string("a_"));
    REQUIRE(astgen.generate("a", "b").substr(0, 4) == std::string("a_b_"));
    REQUIRE(astgen.generate("a", "b", 1, 2.3).substr(0, 10) ==
            std::string("a_b_1_2.3_"));
    REQUIRE(astgen.generateWith("c", "d").substr(0, 4) == std::string("c@d_"));
    REQUIRE(astgen.generateWith("c", "d", 4, 5.6).substr(0, 10) ==
            std::string("c@d_4_5.6_"));
  }
  SECTION("Ir Name Generator") {
    NameGenerator irgen("shp.ir", ".", ".");
    REQUIRE(irgen.generate("a").substr(0, 2) == std::string("shp.ir.a."));
    REQUIRE(irgen.generate("a", "b").substr(0, 4) ==
            std::string("shp.ir.a.b."));
    REQUIRE(irgen.generate("a", "b", 1, 2.3).substr(0, 10) ==
            std::string("shp.ir.a.b.1.2.3."));
    REQUIRE(irgen.generateWith("c", "d").substr(0, 4) ==
            std::string("shp.ir.c.d."));
    REQUIRE(irgen.generateWith("c", "d", 4, 5.6).substr(0, 10) ==
            std::string("shp.ir.c.d.4.5.6."));
  }
}
