// Copyright 2019- <co-lang>
// Apache License Version 2.0

#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Log", "[Log]") {
  SECTION("logging operations") {
    CTRACE("trace test");
    CTRACE("trace test with args: {} {} {}", "hello", 1, 4.281);
    CDEBUG("debug test");
    CDEBUG("debug test with args:{} {} {}", "hello", 1, 4.281);
    CINFO("info test");
    CINFO("info test with args:{} {} {}", "hello", 1, 4.281);
    CERROR("error test");
    CERROR("error test with args:{} {} {}", "hello", 1, 4.281);
  }
}
