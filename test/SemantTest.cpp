// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Semant.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.h"
#include "Token.h"
#include "TokenBuffer.h"
#include "catch2/catch.hpp"

static void go(const char *module) {
  if (!TokenBuffer::pushImport(module)) {
    CASSERT(false, "tokenPushImport {} fail", module);
    return;
  }
  AstProgram *program = nullptr;
  REQUIRE(yyparse(&program) == 0);
  CINFO("go end, program: {}", program ? program->toString() : "null");
  Semant *semant = new Semant(program);
  semant->build();
  semant->check();
}

TEST_CASE("Semant", "[Semant]") {
  SECTION("build & check") {
    // go("test.DslTest1");
    // go("test.DslTest1");
  }
}
