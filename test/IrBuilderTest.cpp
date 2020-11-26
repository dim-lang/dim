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

static void testIrBuilder(const Cowstr &fileName, bool enableFunctionPass) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  SymbolBuilder symbolBuilder;
  SymbolResolver symbolResolver;
  IrBuilder irBuilder(enableFunctionPass);
  PhaseManager pm({&symbolBuilder, &symbolResolver, &irBuilder});
  pm.run(scanner.compileUnit());
  FileWriter fwriter(fileName + ".enableFunctionPass." +
                     Cowstr::from(enableFunctionPass) + ".ll");
  fwriter.write(Cowstr::from(irBuilder.llvmModule()));
}

TEST_CASE("IrBuilder", "[IrBuilder]") {
  SECTION("ir builder without LLVM::FunctionPass") {
    testIrBuilder("test/case/ir-var-def-1.dim", false);
    testIrBuilder("test/case/ir-var-def-2.dim", false);
  }

  SECTION("ir builder with LLVM::FunctionPass") {
    testIrBuilder("test/case/ir-var-def-1.dim", true);
    testIrBuilder("test/case/ir-var-def-2.dim", true);
  }
}
