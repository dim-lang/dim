// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Phase.h"
#include "Scanner.h"
#include "SymbolBuilder.h"
#include "SymbolResolver.h"
#include "catch2/catch.hpp"

static void testIrBuilder(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  SymbolBuilder symbolBuilder;
  SymbolResolver symbolResolver;
  IrBuilder irBuilder(fileName + ".ll");
  PhaseManager pm({&symbolBuilder, &symbolResolver, &irBuilder});
  pm.run(scanner.compileUnit());
}

TEST_CASE("IrBuilder", "[IrBuilder]") {
  SECTION("ir builder") {
    testIrBuilder("test/case/ir-builder-1.nerd");
    // testIrBuilder("test/case/ir-builder-2.nerd");
    // testIrBuilder("test/case/ir-builder-3.nerd");
  }
}
