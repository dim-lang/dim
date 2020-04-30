// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Semantic.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "Token.h"
#include "catch2/catch.hpp"

static void go(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("translateUnit: {}", scanner.translateUnit()->toString());
  SymbolTable symtable;
  Semantic::build(&symtable, scanner.translateUnit());
  Semantic::check(&symtable, scanner.translateUnit());
}

TEST_CASE("Semant", "[Semant]") {
  SECTION("build & check") {
    go("test/case/Parser1.shp");
    go("test/case/Parser2.shp");
  }
}
