// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Log", "[Log]") {
  SECTION("logging operations") {
    INFO("Log Operations");
    FDEBUG_MSG("debug test");
    FDEBUG("debug test with args:{} {} {}", "hello", 1, 4.281);
    FINFO_MSG("info test");
    FINFO("info test with args:{} {} {}", "hello", 1, 4.281);
  }
}
