// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "container/CycleBuffer.h"
#include "Exception.h"
#include "Log.h"
#include "catch2/catch.hpp"

#define C_MAX 100

TEST_CASE("container/CycleBuffer", "[container/CycleBuffer]") {
  SECTION("constructor") {
    DynamicBuffer db;
    char c;
    for (int i = 0; i < C_MAX; i++) {
      c = (char)i;
      db.write(&c, 1);
    }
    const char *cp = db.begin();
    for (int i = 0; i < C_MAX; i++) {
      REQUIRE((int)*cp == i);
      cp = db.next(cp);
    }
  }
}
