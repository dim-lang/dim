// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "catch2/catch.hpp"

TEST_CASE("Logging", "[Logging]") {
  SECTION("logging location") {
    INFO("Logging Location");
    fastype::detail::Location ll("fileName", 1, "main");
    REQUIRE(ll.fileName() == "fileName");
    REQUIRE(ll.lineNumber() == 1);
    REQUIRE(ll.functionName() == "main");
  }

  SECTION("logging operations") {
    INFO("Logging Operations");
    auto a = fastype::LogManager::getLogger("Logging");
    auto b = fastype::LogManager::getLogger("Logging");
    auto c = fastype::LogManager::getLogger("Logging");
    auto d = fastype::LogManager::getLogger("Log");
    REQUIRE(a.get() == b.get());
    REQUIRE(a.get() == c.get());
    REQUIRE(a.get() != d.get());
  }
}
