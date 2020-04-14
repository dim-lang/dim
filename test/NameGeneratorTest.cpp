// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#include "NameGenerator.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("NameGenerator", "[NameGenerator]") {
  SECTION("nameGen") {
    LOG_INFO("nameGen: {}", nameGen("a"));
    LOG_INFO("nameGen: {}", nameGen("a", "b"));
  }
}
