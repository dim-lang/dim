// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "exception/NotFoundException.h"
#include "Logging.h"
#include "Random.h"
#include "catch2/catch.hpp"

#define TEST_MAX 128

TEST_CASE("NotFoundException", "[NotFoundException]") {
  fastype::Random::initialize();

  SECTION("Exception") {
    for (int i = 0; i < TEST_MAX; i++) {
      try {
        F_THROW(fastype::NotFoundException, "{}", i);
      } catch (fastype::NotFoundException &e) {
        F_ERROR("not found: {}", e.toString());
      }
    }
    REQUIRE(true);
  }
}
