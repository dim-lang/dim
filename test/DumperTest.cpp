// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Dumper.h"
#include "Ast.h"
#include "Scanner.h"
#include "SymbolBuilder.h"
#include "SymbolResolver.h"
#include "catch2/catch.hpp"
#include "fmt/format.h"
#include "iface/Phase.h"
#include "infra/Log.h"

static void testDumper(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);

  SymbolBuilder builder;
  SymbolResolver resolver;
  Dumper dumper;

  PhaseManager pm({&builder, &resolver, &dumper});
  pm.run(scanner.compileUnit());
  LOG_INFO("dump {}", fileName);
  LOG_INFO("\n{}", Cowstr::join(dumper.dump(), "\n"));
}

TEST_CASE("Dumper", "[Dumper]") {
  SECTION("dump ast") {
    testDumper("test/case/parse-1.dim");
    testDumper("test/case/parse-2.dim");
    testDumper("test/case/parse-3.dim");
    testDumper("test/case/parse-4.dim");
  }
}
