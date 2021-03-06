// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "SymbolResolver.h"
#include "Ast.h"
#include "Scanner.h"
#include "Symbol.h"
#include "SymbolBuilder.h"
#include "catch2/catch.hpp"
#include "iface/Phase.h"

static void testSymbolResolver(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  SymbolBuilder builder;
  SymbolResolver resolver;
  PhaseManager pm({&builder, &resolver});
  pm.run(scanner.compileUnit());
}

TEST_CASE("SymbolResolver", "[SymbolResolver]") {
  SECTION("resolve symbol") {
    testSymbolResolver("test/case/parse-1.dim");
    testSymbolResolver("test/case/parse-2.dim");
    testSymbolResolver("test/case/parse-3.dim");
    testSymbolResolver("test/case/parse-4.dim");
  }
}
