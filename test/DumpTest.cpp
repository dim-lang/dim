// Copyright 2019- <ac-lang>
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
  CINFO("dump ast: {} {}", fileName, dumpAst(scanner.translateUnit()));
  SymbolTable symtable;
  Semantic::build(&symtable, scanner.translateUnit());
  Semantic::check(&symtable, scanner.translateUnit());
  CINFO("dump symbol:{} {}", fileName, dumpSymbol(symtable.gss()));
  CINFO("dump type:{} {}", fileName, dumpType(symtable.gts()));
}

TEST_CASE("Dump", "[Dump]") {
  SECTION("dump for Ast") {
    go("test/case/DslTest1.co");
    go("test/case/DslTest2.co");
  }
}
