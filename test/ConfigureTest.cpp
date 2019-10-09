// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Configure.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <cstring>

TEST_CASE("Configure", "[Configure]") {
  SECTION("Project Version Info") {
    F_INFO_MSG("Project Version Info");
    REQUIRE(std::strlen(PROJECT_NAME) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MAJOR) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MINOR) >= 0);
    REQUIRE(std::strlen(PROJECT_VERSION_PATCH) >= 0);
    F_INFO("PROJECT_NAME:{}", PROJECT_NAME);
    F_INFO("PROJECT_VERSION:{}", PROJECT_VERSION);
    F_INFO("PROJECT_VERSION_MAJOR:{}", PROJECT_VERSION_MAJOR);
    F_INFO("PROJECT_VERSION_MINOR:{}", PROJECT_VERSION_MINOR);
    F_INFO("PROJECT_VERSION_PATCH:{}", PROJECT_VERSION_PATCH);
  }
}
