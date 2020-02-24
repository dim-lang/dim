// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Log.h"
#include "Parser.h"
#include "Token.h"
#include "TokenBuffer.h"
#include "catch2/catch.hpp"

static void go(const char *module) {
  REQUIRE(TokenBuffer::pushImport(module) == 1);
  AstProgram *program = nullptr;
  REQUIRE(yyparse(&program) == 0);
  CINFO("dump ast: {}", dump(program));
  Semant *semant = new Semant(program);
  semant->build();
  semant->check();
  CINFO("dump symbol: {}", dump(semant->globalSymbolTable()));
  CINFO("dump type: {}", dump(semant->globalTypeTable()));
}

TEST_CASE("Dump", "[Dump]") {
  SECTION("dump for Ast") {
    go("test.case.DslTest1");
    go("test.case.DslTest2");
  }
}
