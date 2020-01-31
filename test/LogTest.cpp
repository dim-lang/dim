// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Log", "[Log]") {
  SECTION("logging operations") {
    INFO("Log Operations");
    LOG_DEBUG_MSG("debug test");
    LOG_DEBUG("debug test with args:{} {} {}", "hello", 1, 4.281);
    LOG_INFO_MSG("info test");
    LOG_INFO("info test with args:{} {} {}", "hello", 1, 4.281);
  }
}
