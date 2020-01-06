// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Logging.h"
#include "catch2/catch.hpp"

TEST_CASE("Main", "[Main]") {
  SECTION("Main Initialize") {
    Logging::initialize(".", "fastype-test");
    FINFO_MSG("fastype-test starting...");
  }
}
