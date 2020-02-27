// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Semant.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.h"
#include "Token.h"
#include "catch2/catch.hpp"

static void go(const char *module) {
  Scanner scanner;
  REQUIRE(scanner.push(module) == 1);
  REQUIRE(yyparse(&scanner) == 0);
  CINFO("program: {}", scanner.program->toString());
  Semant *semant = new Semant(scanner.program);
  semant->build();
  semant->check();
}

TEST_CASE("Semant", "[Semant]") {
  SECTION("build & check") {
    // go("test.DslTest1");
    // go("test.DslTest1");
  }
}
