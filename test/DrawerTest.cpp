// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Drawer.h"
#include "Ast.h"
#include "Scanner.h"
#include "SymbolBuilder.h"
#include "catch2/catch.hpp"

static void testDrawer(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  SymbolBuilder builder;
  builder.run(scanner.compileUnit());
  Drawer drawer(fileName + ".dot");
  drawer.run(scanner.compileUnit());
}

TEST_CASE("Drawer", "[Drawer]") {
  SECTION("ast/symbol graph") {
    testDrawer("test/case/parse-1.nerd");
    testDrawer("test/case/parse-2.nerd");
    testDrawer("test/case/parse-3.nerd");
  }
}
