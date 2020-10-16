// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Drawer.h"
#include "Ast.h"
#include "Scanner.h"
#include "Symbol.h"
#include "catch2/catch.hpp"

static void drawTest(const std::string &fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  REQUIRE(Drawer::draw(scanner.compileUnit(), fileName + ".dot") == 0);
}

TEST_CASE("Drawer", "[Drawer]") {
  SECTION("ast/symbol graph") {
    drawTest("test/case/parse-1.nerd");
    drawTest("test/case/parse-2.nerd");
    drawTest("test/case/parse-3.nerd");
  }
}
