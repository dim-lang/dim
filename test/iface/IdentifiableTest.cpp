// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "iface/Identifiable.h"
#include "catch2/catch.hpp"
#include "infra/Log.h"

TEST_CASE("Identifiable", "[Identifiable]") {
  SECTION("Identifiable") {
    Identifiable a;
    Identifiable b;
    Identifiable c;
    REQUIRE(a.identifier() != b.identifier());
    REQUIRE(b.identifier() != c.identifier());
    REQUIRE(a.identifier() != c.identifier());
    REQUIRE(a.decIdentifier() == std::to_string(a.identifier()));
    REQUIRE(b.decIdentifier() == std::to_string(b.identifier()));
    REQUIRE(c.decIdentifier() == std::to_string(c.identifier()));
  }
}
