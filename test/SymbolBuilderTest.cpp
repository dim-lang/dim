// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "SymbolBuilder.h"
#include "Ast.h"
#include "Scanner.h"
#include "Symbol.h"
#include "catch2/catch.hpp"

static void testSymbolBuilder(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  SymbolBuilder builder;
  builder.run(scanner.compileUnit());
}

TEST_CASE("SymbolBuilder", "[SymbolBuilder]") {
  SECTION("build symbol") {
    testSymbolBuilder("test/case/parse-1.dim");
    testSymbolBuilder("test/case/parse-2.dim");
    testSymbolBuilder("test/case/parse-3.dim");
    testSymbolBuilder("test/case/parse-4.dim");
  }
}
