// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Main", "[Main]") {
  Log::initialize("shepherd-test");
  SECTION("Main Initialize") { LOG_INFO("shepherd-test starting..."); }
}
