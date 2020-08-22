// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Buffer.h"
#include "Log.h"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include "parser.tab.h"
#include <cstdio>

static void testParse(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("test parse end, translateUnit: {}",
           scanner.translateUnit()->toString());
}

TEST_CASE("parser", "[parser]") {
  SECTION("parser") {
    testParse("test/case/parser1.shp");
    testParse("test/case/parser2.shp");
  }
}
