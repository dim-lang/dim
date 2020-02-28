// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Semant.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "Token.h"
#include "catch2/catch.hpp"

static void go(const char *fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  CINFO("program: {}", scanner.program()->toString());
  Semant *semant = new Semant(scanner.program());
  semant->build();
  semant->check();
}

TEST_CASE("Semant", "[Semant]") {
  SECTION("build & check") {
    // go("test.DslTest1");
    // go("test.DslTest1");
  }
}
