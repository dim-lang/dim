// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Main", "[Main]") {
  Log::initialize("rsc-test");
  SECTION("Main Initialize") { LOG_INFO("rsc-test starting..."); }
}
