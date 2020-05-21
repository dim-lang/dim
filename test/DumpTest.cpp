// Copyright 2019- <shepherd-lang>
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
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  IrContext context(fileName);
  IrTranslateUnit tunit(scanner.translateUnit());
  LOG_INFO("dump ir:{} {}", fileName, tunit.dumpCodeGen(&context));
}

TEST_CASE("Dump", "[Dump]") {
  SECTION("dump Ast/Symbol/Type") {
    testAST("test/case/Parser1.shp");
    testAST("test/case/Parser2.shp");
  }
  SECTION("dump Ir") {
    // testIr("test/case/IrTest1.shp");
    // testIr("test/case/IrTest2.shp");
    testIr("test/case/IrTest3.shp");
  }
}
