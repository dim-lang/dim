// Copyright 2019- <rooster-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Main", "[Main]") {
  Log::initialize("rooster-test");
  SECTION("Main Initialize") { LOG_INFO("rooster-test starting..."); }
}
