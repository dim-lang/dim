// Copyright 2019- <fastype.org>
// Apache License Version 2.0

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
  int r = yyparse();
  FINFO("parserGo {} ending with {} ...", module, r);
}

TEST_CASE("Parser", "[Parser]") {
  SECTION("Parser") {
    parserGo("test.DslTest1");
    parserGo("test.DslTest2");
  }
}
