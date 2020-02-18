// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "Token.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void go(const char *module) {
  if (!tokenPushImport(module)) {
    CASSERT(false, "tokenPushImport {} fail", module);
    return;
  }
  CINFO("go {} starting...", module);
  REQUIRE(yyparse(nullptr) == 0);
  CINFO("go {} ending...", module);
}

TEST_CASE("Parser", "[Parser]") {
  SECTION("Parser") {
    go("test.DslTest1");
    go("test.DslTest2");
  }
}
