// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "Semantic.h"
#include "Token.h"
#include "catch2/catch.hpp"

static void go(const char *fileName) {
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

TEST_CASE("Dump", "[Dump]") {
  SECTION("dump for Ast") {
    go("test/case/Parser1.rsc");
    go("test/case/Parser2.rsc");
  }
}
