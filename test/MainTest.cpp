// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Main", "[Main]") {
  Log::initialize("ac-test");
  SECTION("Main Initialize") { CINFO("ac-test starting..."); }
}
