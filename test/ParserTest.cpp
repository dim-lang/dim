// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Parser.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "Token.h"
#include "TokenBuffer.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void go(const char *module) {
  if (!TokenBuffer::pushImport(module)) {
    CASSERT(false, "tokenPushImport {} fail", module);
    return;
  }
  AstProgram *program = nullptr;
  REQUIRE(yyparse(&program) == 0);
  CINFO("go end, program: {}", program ? program->toString() : "null");
}

TEST_CASE("Parser", "[Parser]") {
  SECTION("Parser") {
    go("test.case.DslTest1");
    go("test.case.DslTest2");
  }
}
