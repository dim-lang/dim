// Copyright 2019- <rec-lang>
// Apache License Version 2.0

#include "Configure.h"
#include "Log.h"
#include "catch2/catch.hpp"
#include <cstring>

TEST_CASE("Configure", "[Configure]") {
  SECTION("Project Version Info") {
    REQUIRE(std::strlen(PROJECT_NAME) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MAJOR) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MINOR) >= 0);
    REQUIRE(std::strlen(PROJECT_VERSION_PATCH) >= 0);
    LOG_INFO("PROJECT_NAME:{}", PROJECT_NAME);
    LOG_INFO("PROJECT_VERSION:{}", PROJECT_VERSION);
    LOG_INFO("PROJECT_VERSION_MAJOR:{}", PROJECT_VERSION_MAJOR);
    LOG_INFO("PROJECT_VERSION_MINOR:{}", PROJECT_VERSION_MINOR);
    LOG_INFO("PROJECT_VERSION_PATCH:{}", PROJECT_VERSION_PATCH);
  }
}
