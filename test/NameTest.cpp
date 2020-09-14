// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Name.h"
#include "Log.h"
#include "catch2/catch.hpp"

TEST_CASE("Name", "[Name]") {
  SECTION("Nameable") {
    Nameable a1;
    Nameable a2("a2");
    REQUIRE(a1.name().length() == 0);
    REQUIRE(a2.name().length() == 2);
    REQUIRE(a2.name().str() == "a2");
  }
  SECTION("NameGenerator") {
    AstGraphNameGenerator g1;
    SymbolNameGenerator g2;
    IrNameGenerator g3;
    LOG_INFO("g1.from:{} -> {}", "hello", g1.from("hello"));
    LOG_INFO("g2.from:{} -> {}", "hello", g2.from("hello"));
    LOG_INFO("g3.from:{} -> {}", "hello", g3.from("hello"));
  }
}