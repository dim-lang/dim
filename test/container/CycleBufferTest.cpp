// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "container/CycleBuffer.h"
#include "Log.h"
#include "catch2/catch.hpp"

#define C_MIN 0
#define C_MAX 100
#define BUF_SIZE 100

TEST_CASE("container/CycleBuffer", "[container/CycleBuffer]") {
  SECTION("constructor") {
    DynamicBuffer db;
    FixedBuffer fb(BUF_SIZE);
  }
  SECTION("DynamicBuffer foreach") {
    DynamicBuffer db;
    LOG_INFO("db-1: {}", db.toString());
    char c;
    for (int i = C_MIN; i < C_MAX; i++) {
      c = (char)i;
      REQUIRE(db.write(&c, 1) == 0);
    }
    for (int i = C_MIN; i < C_MAX; i++) {
      c = (char)i;
      REQUIRE(db.read(&c, 1) == 1);
    }
    LOG_INFO("db-2: {}", db.toString());
    const char *cp = db.begin();
    for (int i = C_MIN; i < C_MAX; i++) {
      REQUIRE((int)*cp == i);
      cp = db.next(cp);
    }
    cp = db.rbegin();
    for (int i = C_MAX - 1; i >= C_MIN; i--) {
      REQUIRE((int)*cp == i);
      cp = db.prev(cp);
    }
  }
}
