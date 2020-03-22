// Copyright 2019- <co-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Main", "[Main]") {
  Log::initialize("coli-test");
  SECTION("Main Initialize") { CINFO("coli-test starting..."); }
}
