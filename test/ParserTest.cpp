// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Parser.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "Token.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void go(const char *module) {
  Scanner scanner;
  REQUIRE(scanner.push(module) == 1);
  REQUIRE(yyparse(scanner.scaninfo, &scanner) == 0);
  CINFO("go end, program: {}", scanner.program->toString());
}

TEST_CASE("Parser", "[Parser]") {
  SECTION("Parser") {
    go("test.case.DslTest1");
    go("test.case.DslTest2");
  }
}
