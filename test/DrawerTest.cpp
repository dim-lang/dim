// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Drawer.h"
#include "Ast.h"
#include "Scanner.h"
#include "SymbolBuilder.h"
#include "SymbolResolver.h"
#include "catch2/catch.hpp"
#include "iface/Phase.h"

static void testDrawer(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);

  SymbolBuilder builder;
  SymbolResolver resolver;
  Drawer drawer(fileName + ".dot");

  PhaseManager pm({&builder, &resolver, &drawer});
  pm.run(scanner.compileUnit());
}

TEST_CASE("Drawer", "[Drawer]") {
  SECTION("ast graph") {
    testDrawer("test/case/parse-1.dim");
    testDrawer("test/case/parse-2.dim");
    testDrawer("test/case/parse-3.dim");
    testDrawer("test/case/parse-4.dim");
  }
}
