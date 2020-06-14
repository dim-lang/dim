// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Parser.h"
#include "Ast.h"
#include "Buffer.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void go(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("go end, translateUnit: {}", scanner.translateUnit()->toString());
}

TEST_CASE("Parser", "[Parser]") {
  SECTION("Parser") {
    go("test/case/Parser1.shp");
    go("test/case/Parser2.shp");
  }
}
