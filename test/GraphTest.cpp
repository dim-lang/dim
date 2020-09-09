// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Scanner.h"
#include "catch2/catch.hpp"

static void dumpAst(const std::string &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  REQUIRE(Graph::drawAst(scanner.compileUnit(), fileName + ".dot") == 0);
}

TEST_CASE("Graph", "[Graph]") {
  SECTION("ast graph") {
    dumpAst("test/case/parse-1.nerd");
    dumpAst("test/case/parse-2.nerd");
    dumpAst("test/case/parse-3.nerd");
  }
}
