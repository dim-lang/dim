// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#include "Parser.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "Scanner.h"
#include "Token.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void go(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  CINFO("go end, translateUnit: {}", scanner.translateUnit()->toString());
}

TEST_CASE("Parser", "[Parser]") {
  SECTION("Parser") {
    go("test/case/DslTest1.co");
    go("test/case/DslTest2.co");
  }
}
