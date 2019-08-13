// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Define.h"
#include "catch2/catch.hpp"
#include <cstring>

TEST_CASE("Define", "[Define]") {
  SECTION("Project Version Info") {
    INFO("Project Version Info");
    REQUIRE(std::strlen(PROJECT_NAME) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MAJOR) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MINOR) >= 0);
    REQUIRE(std::strlen(PROJECT_VERSION_PATCH) >= 0);
  }
}
