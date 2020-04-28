// Copyright 2019- <rec-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Main", "[Main]") {
  Log::initialize("rec-test");
  SECTION("Main Initialize") { LOG_INFO("rec-test starting..."); }
}
