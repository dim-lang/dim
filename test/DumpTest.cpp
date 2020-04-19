// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Ast.h"
#include "Ir.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "Semantic.h"
#include "Token.h"
#include "catch2/catch.hpp"

static void testAST(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("dump ast: {} {}", fileName, dumpAst(scanner.translateUnit()));
  SymbolTable symtable;
  Semantic::build(&symtable, scanner.translateUnit());
  Semantic::check(&symtable, scanner.translateUnit());
  LOG_INFO("dump symbol:{} {}", fileName, dumpSymbol(symtable.gss()));
  LOG_INFO("dump type:{} {}", fileName, dumpType(symtable.gts()));
}

static void testIr(const char *fileName) {
  Scanner scanner;
  IrContext context;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("dump ir: {} {}", fileName, dumpIr(scanner.translateUnit()));
}

TEST_CASE("Dump", "[Dump]") {
  SECTION("dump Ast/Symbol/Type") {
    testAST("test/case/Parser1.rsc");
    testAST("test/case/Parser2.rsc");
  }
  SECTION("dump Ir") { testIr("test/case/IrTest1.rsc"); }
}
