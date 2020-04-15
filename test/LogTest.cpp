// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Log", "[Log]") {
  SECTION("logging operations") {
    LOG_TRACE("trace test");
    LOG_TRACE("trace test with args: {} {} {}", "hello", 1, 4.281);
    LOG_DEBUG("debug test");
    LOG_DEBUG("debug test with args:{} {} {}", "hello", 1, 4.281);
    LOG_INFO("info test");
    LOG_INFO("info test with args:{} {} {}", "hello", 1, 4.281);
    LOG_ERROR("error test");
    LOG_ERROR("error test with args:{} {} {}", "hello", 1, 4.281);
  }
}
