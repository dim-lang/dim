// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "catch2/catch.hpp"

TEST_CASE("Logging", "[Logging]") {
  SECTION("logging operations") {
    INFO("Logging Operations");
    F_LOG_INIT(".", "fastype-test");
    F_DEBUG("debug test");
    F_DEBUGF("debug test with args:{} {} {}", "hello", 1, 4.281);
    F_INFO("info test");
    F_INFOF("info test with args:{} {} {}", "hello", 1, 4.281);
    F_ERROR("error test");
    F_ERRORF("error test with args:{} {} {}", "hello", 1, 4.281);
  }
}
