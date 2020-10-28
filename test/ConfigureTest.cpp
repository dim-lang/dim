// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Configure.h"
#include "catch2/catch.hpp"
#include "infra/Log.h"
#include <cstring>

TEST_CASE("Configure", "[Configure]") {
  SECTION("Project Version Info") {
    REQUIRE(std::strlen(PROJECT_NAME) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MAJOR) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MINOR) >= 0);
    REQUIRE(std::strlen(PROJECT_VERSION_PATCH) >= 0);
  }
}
