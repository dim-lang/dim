// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Drawer.h"
#include "Ast.h"
#include "Phase.h"
#include "Scanner.h"
#include "SymbolBuilder.h"
#include "SymbolReviewer.h"
#include "catch2/catch.hpp"

static void testDrawer(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);

  SymbolBuilder builder;
  SymbolReviewer reviewer;
  Drawer drawer(fileName + ".dot");

  PhaseManager pm({&builder, &reviewer, &drawer});
  pm.run(scanner.compileUnit());
}

TEST_CASE("Drawer", "[Drawer]") {
  SECTION("ast/symbol graph") {
    testDrawer("test/case/parse-1.nerd");
    testDrawer("test/case/parse-2.nerd");
    testDrawer("test/case/parse-3.nerd");
  }
}
