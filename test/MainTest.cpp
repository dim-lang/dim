// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Main", "[Main]") {
  SECTION("Main Initialize") {
    Log::initialize("coli-test");
    CINFO("coli-test starting...");
  }
}
