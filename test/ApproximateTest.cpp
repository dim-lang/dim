// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Approximate.h"
#include "Log.h"
#include "catch2/catch.hpp"
#include <cstring>

TEST_CASE("Approximate", "[Approximate]") {
  SECTION("float double") {
    REQUIRE(!Approximate::eq((float)0.0001F, (float)0.0009F));
    REQUIRE(Approximate::lt((float)0.0001F, (float)0.00011F));
    REQUIRE(Approximate::gt((float)0.00012F, (float)0.00011F));
    REQUIRE(Approximate::eq((float)0.00013F, (float)0.00013F));
    REQUIRE(Approximate::lt((float)0.00013F, (float)0.000131F));
    REQUIRE(Approximate::gt((float)0.0001311F, (float)0.000131F));
    REQUIRE(!Approximate::eq((double)0.000011F, (double)0.00001F));
    REQUIRE(Approximate::lt((double)0.000011F, (double)0.000011001F));
    REQUIRE(Approximate::gt((double)0.000011002F, (double)0.000011001F));
    REQUIRE(Approximate::eq((double)0.000000000001, (double)0.000000000001));
    REQUIRE(Approximate::lt((double)0.0000000000010001,
                            (double)0.00000000000100011));
    REQUIRE(Approximate::gt((double)0.000000000001000111,
                            (double)0.00000000000100011));
  }
}
