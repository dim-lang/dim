// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Log", "[Log]") {
  SECTION("logging operations") {
    INFO("Log Operations");
    CDEBUG_MSG("debug test");
    CDEBUG("debug test with args:{} {} {}", "hello", 1, 4.281);
    CINFO_MSG("info test");
    CINFO("info test with args:{} {} {}", "hello", 1, 4.281);
  }
}
