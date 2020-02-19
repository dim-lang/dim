// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Log.h"
#include "TokenExt.h"
#include "catch2/catch.hpp"

static void dumpAst(const char *module) {
  if (!tokenPushImport(module)) {
    CASSERT(false, "tokenPushImport {} fail", module);
    return;
  }
  AstDeclarationList *program = nullptr;
  REQUIRE(yyparse(&program) == 0);
  dump(std::string("TestDumpAst") + std::string(module) + ".txt", program);
}

TEST_CASE("Dump", "[Dump]") {
  SECTION("dump for Ast") {
    dumpAst("test.DslTest1");
    dumpAst("test.DslTest2");
  }
}
