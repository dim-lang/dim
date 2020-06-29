// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Buffer.h"
#include "Ir.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "catch2/catch.hpp"

static void testAST(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("dump ast: {} {}", fileName, dumpAst(scanner.translateUnit()));
  IrContext context(fileName);
  IrTranslateUnit tunit(&context, scanner.translateUnit());
  tunit.buildSymbol();
  LOG_INFO("dump symbol:{} {}", fileName,
           dumpScope(Scope::make_snode(context.symbolTable()->global,
                                       ScopeType::ty_global(),
                                       scanner.translateUnit())));
  LOG_INFO("dump source:{}\n{}", fileName,
           dumpSource(fileName, scanner.translateUnit()->position()));
}

TEST_CASE("Dump", "[Dump]") {
  SECTION("dump Ast/Symbol/Type/Source") {
    testAST("test/case/Parser1.shp");
    testAST("test/case/Parser2.shp");
  }
}
