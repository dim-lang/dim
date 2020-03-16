// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "Semant.h"
#include "Token.h"
#include "catch2/catch.hpp"

static void go(const char *fileName) {
  SymbolTable symtable;
  Scanner scanner(&symtable);
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  CINFO("dump ast: {} {}", fileName, dumpAst(scanner.translateUnit()));
  Semant *semant = new Semant(scanner.translateUnit());
  semant->build();
  semant->check();
  CINFO("dump symbol:{} {}", fileName, dumpSymbol(semant->globalSymbolTable()));
  CINFO("dump type:{} {}", fileName, dumpType(semant->globalTypeTable()));
}

TEST_CASE("Dump", "[Dump]") {
  SECTION("dump for Ast") {
    go("test/case/DslTest1.co");
    go("test/case/DslTest2.co");
  }
}
