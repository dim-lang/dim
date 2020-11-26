// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "infra/Counter.h"
#include "catch2/catch.hpp"

#define MAX 1024ULL

TEST_CASE("Counter", "[Counter]") {
  SECTION("Counter") {
    Counter c;
    for (unsigned long long i = 1; i < MAX; i++) {
      REQUIRE(c.count() == i);
    }
  }
}
