// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Dumper.h"
#include "Ast.h"
#include "Phase.h"
#include "Scanner.h"
#include "SymbolBuilder.h"
#include "SymbolResolver.h"
#include "catch2/catch.hpp"
#include "fmt/format.h"

static void testDumper(const Cowstr &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);

  SymbolBuilder builder;
  SymbolResolver resolver;
  Dumper dumper;

  PhaseManager pm({&builder, &resolver, &drawer});
  pm.run(scanner.compileUnit());
  fmt::print("dump {}\n", fileName);
  for (int i = 0; i < (int)dumper.dumps().size(); i++) {
    fmt::print("{}\n", dumper.dumps()[i]);
  }
}

TEST_CASE("Dumper", "[Dumper]") {
  SECTION("dump ast/symbol") {
    testDumper("test/case/parse-1.nerd");
    testDumper("test/case/parse-2.nerd");
    testDumper("test/case/parse-3.nerd");
    testDumper("test/case/parse-4.nerd");
  }
}
