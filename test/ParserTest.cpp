// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "Token.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void parserGo(const char *module) {
  if (!tokenImportFile(module)) {
    FCHECK(false, "tokenImportFile {} fail", module);
    return;
  }
  FINFO("parserGo {} starting...", module);
  REQUIRE(yyparse() == 0);
  FINFO("parserGo {} ending ...", module);
}

TEST_CASE("Parser", "[Parser]") {
  SECTION("Parser") {
    parserGo("test.DslTest1");
    parserGo("test.DslTest2");
  }
}
