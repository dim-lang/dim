// Copyright 2019- <collie.org>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Main", "[Main]") {
  Log::initialize("collie-test");
  SECTION("Main Initialize") { CINFO("collie-test starting..."); }
}
