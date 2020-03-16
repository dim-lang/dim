// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Semantic.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "Token.h"
#include "catch2/catch.hpp"

static void go(const char *fileName) {
  SymbolTable symtable;
  Scanner scanner(&symtable);
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  CINFO("translateUnit: {}", scanner.translateUnit()->toString());
  Semant *semant = new Semant(scanner.translateUnit());
  semant->build();
  semant->check();
}

TEST_CASE("Semant", "[Semant]") {
  SECTION("build & check") {
    // go("test.DslTest1");
    // go("test.DslTest1");
  }
}
