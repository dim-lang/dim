// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Graph.h"
#include "Ast.h"
#include "Scanner.h"
#include "Symbol.h"
#include "catch2/catch.hpp"

static void drawTest(const std::string &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  REQUIRE(Graph::drawAst(scanner.compileUnit(), fileName + ".ast.dot") == 0);
  std::shared_ptr<Scope> scope = Scope::from(scanner.compileUnit());
  REQUIRE(Graph::drawSymbol(scope, fileName + ".symbol.dot") == 0);
}

TEST_CASE("Graph", "[Graph]") {
  SECTION("ast/symbol graph") {
    drawTest("test/case/parse-1.nerd");
    drawTest("test/case/parse-2.nerd");
    drawTest("test/case/parse-3.nerd");
  }
}
