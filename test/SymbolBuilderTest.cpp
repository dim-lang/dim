// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "SymbolBuilder.h"
#include "Ast.h"
#include "Log.h"
#include "Scanner.h"
#include "Symbol.h"
#include "catch2/catch.hpp"

static void testSymbolBuilder(const char *fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("compileUnit: {}", scanner.compileUnit()->name());
  SymbolBuilder builder;
  builder.run(scanner.compileUnit());
}

TEST_CASE("SymbolBuilder", "[SymbolBuilder]") {
  SECTION("symbol") {
    testSymbolBuilder("test/case/parse-1.nerd");
    testSymbolBuilder("test/case/parse-2.nerd");
    testSymbolBuilder("test/case/parse-3.nerd");
  }
}
