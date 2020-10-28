// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "infra/Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Log", "[Log]") {
  SECTION("logging") {
    LOG_TRACE("trace test");
    LOG_TRACE("trace test with args: {} {} {}", "hello", 1, 4.281);
    LOG_DEBUG("debug test");
    LOG_DEBUG("debug test with args:{} {} {}", "hello", 1, 4.281);
    LOG_INFO("info test");
    LOG_INFO("info test with args:{} {} {}", "hello", 1, 4.281);
    LOG_ERROR("error test");
    LOG_ERROR("error test with args:{} {} {}", "hello", 1, 4.281);
    try {
      LOG_ASSERT(false, "this is a wrong message which throw a string");
    } catch (const Cowstr &e) {
      LOG_ERROR("catch it:{}", e);
    }
  }
}
