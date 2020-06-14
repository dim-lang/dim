// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Semantic.h"
#include "Ast.h"
#include "Buffer.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "catch2/catch.hpp"

static void go(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("translateUnit: {}", scanner.translateUnit()->toString());
  SymbolManager smanager;
  Semantic::build(&smanager, scanner.translateUnit());
  Semantic::check(&smanager, scanner.translateUnit());
}

TEST_CASE("Semant", "[Semant]") {
  SECTION("build & check") {
    go("test/case/Parser1.shp");
    go("test/case/Parser2.shp");
  }
}
