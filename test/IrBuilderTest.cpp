// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Scanner.h"
#include "SymbolBuilder.h"
#include "SymbolResolver.h"
#include "catch2/catch.hpp"
#include "iface/Phase.h"
#include "infra/Cowstr.h"
#include "infra/Files.h"

static void testIrBuilder(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  SymbolBuilder symbolBuilder;
  SymbolResolver symbolResolver;
  IrBuilder irBuilder;
  PhaseManager pm({&symbolBuilder, &symbolResolver, &irBuilder});
  pm.run(scanner.compileUnit());
  FileWriter fwriter(fileName + ".ll");
  fwriter.write(Cowstr::from(irBuilder.llvmModule()));
}

TEST_CASE("IrBuilder", "[IrBuilder]") {
  SECTION("ir builder") {
    testIrBuilder("test/case/ir-var-def-1.dim");
    // testIrBuilder("test/case/ir-builder-2.dim");
    // testIrBuilder("test/case/ir-builder-3.dim");
  }
}
