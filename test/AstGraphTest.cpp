// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "AstGraph.h"
#include "Scanner.h"
#include "catch2/catch.hpp"

static void dumpAst(const std::string &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  AstGraph dumper(scanner.compileUnit);
  dumper.write(fileName + ".dot");
}

TEST_CASE("AstGraph", "[AstGraph]") {
  SECTION("dump ast") {
    dumpAst("test/case/parse-1.nerd");
    dumpAst("test/case/parse-2.nerd");
  }
}
