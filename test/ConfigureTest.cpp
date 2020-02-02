// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Configure.h"
#include "Log.h"
#include "catch2/catch.hpp"
#include <cstring>

TEST_CASE("Configure", "[Configure]") {
  SECTION("Project Version Info") {
    CINFO_MSG("Project Version Info");
    REQUIRE(std::strlen(PROJECT_NAME) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MAJOR) > 0);
    REQUIRE(std::strlen(PROJECT_VERSION_MINOR) >= 0);
    REQUIRE(std::strlen(PROJECT_VERSION_PATCH) >= 0);
    CINFO("PROJECT_NAME:{}", PROJECT_NAME);
    CINFO("PROJECT_VERSION:{}", PROJECT_VERSION);
    CINFO("PROJECT_VERSION_MAJOR:{}", PROJECT_VERSION_MAJOR);
    CINFO("PROJECT_VERSION_MINOR:{}", PROJECT_VERSION_MINOR);
    CINFO("PROJECT_VERSION_PATCH:{}", PROJECT_VERSION_PATCH);
  }
}
