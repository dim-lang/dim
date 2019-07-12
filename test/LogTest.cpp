// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Log Location", "[LogLocation]") {
  INFO("Log Location");
  fastype::detail::LogLocation ll("fileName", 1, "main");
  REQUIRE(ll.fileName == "fileName");
  REQUIRE(ll.lineNumber == 1);
  REQUIRE(ll.functionName == "main");
}

TEST_CASE("Log Operations", "[Log]") {
  INFO("Log Operations");
  auto a = fastype::LogManager::getLogger("Log");
  auto b = fastype::LogManager::getLogger("Log");
  auto c = fastype::LogManager::getLogger("Log");
  REQUIRE(a.get() == b.get());
  REQUIRE(a.get() == c.get());
}