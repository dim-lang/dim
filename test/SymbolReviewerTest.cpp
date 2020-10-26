// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "SymbolReviewer.h"
#include "Ast.h"
#include "Log.h"
#include "Phase.h"
#include "Scanner.h"
#include "Symbol.h"
#include "SymbolBuilder.h"
#include "catch2/catch.hpp"

static void testSymbolReviewer(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);

  SymbolBuilder builder;
  SymbolReviewer reviewer;

  PhaseManager pm({&builder, &reviewer});
  pm.run(scanner.compileUnit());
}

TEST_CASE("SymbolReviewer", "[SymbolReviewer]") {
  SECTION("review symbol") {
    testSymbolReviewer("test/case/parse-1.nerd");
    testSymbolReviewer("test/case/parse-2.nerd");
    testSymbolReviewer("test/case/parse-3.nerd");
  }
}
