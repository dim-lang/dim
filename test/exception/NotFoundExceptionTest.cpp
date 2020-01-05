// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "exception/NotFoundException.h"
#include "Logging.h"
#include "Random.h"
#include "catch2/catch.hpp"

TEST_CASE("NotFoundException", "[NotFoundException]") {
  fastype::Random::initialize();

  SECTION("Exception") {
    for (int i = 0; i < 128; i++) {
      try {
        F_THROW(fastype::NotFoundException, "{}", i);
        REQUIRE(false);
      } catch (fastype::NotFoundException &e) {
        REQUIRE(true);
      }
    }
  }
}
