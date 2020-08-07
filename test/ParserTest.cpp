// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Parser.h"
#include "Ast.h"
#include "Buffer.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void testParse(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("test parse end, translateUnit: {}",
           scanner.translateUnit()->toString());
}

TEST_CASE("Parser", "[Parser]") {
  SECTION("Parser") {
    testParse("test/case/Parser1.shp");
    testParse("test/case/Parser2.shp");
  }
}
