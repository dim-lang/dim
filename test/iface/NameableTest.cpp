// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "iface/Nameable.h"
#include "catch2/catch.hpp"
#include "infra/Log.h"

TEST_CASE("Nameable", "[Nameable]") {
  SECTION("Nameable") {
    Nameable a1;
    Nameable a2("a2");
    Nameable a3("a3");
    REQUIRE(a1.name().length() == 0);
    REQUIRE(a2.name().length() == 2);
    REQUIRE(a2.name().str() == "a2");
    REQUIRE(a3.name().str() == "a3");
  }
}
