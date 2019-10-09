// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "catch2/catch.hpp"

TEST_CASE("Logging", "[Logging]") {
  SECTION("logging operations") {
    INFO("Logging Operations");
    F_DEBUG_MSG("debug test");
    F_DEBUG("debug test with args:{} {} {}", "hello", 1, 4.281);
    F_INFO_MSG("info test");
    F_INFO("info test with args:{} {} {}", "hello", 1, 4.281);
    F_ERROR_MSG("error test");
    F_ERROR("error test with args:{} {} {}", "hello", 1, 4.281);
  }
}
