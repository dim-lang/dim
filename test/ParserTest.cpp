// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void parse(const char *fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("parser parse:{} ast:{}", scanner.compileUnit->toString());
}

TEST_CASE("parser", "[parser]") {
  SECTION("parser") {
    parse("test/case/parse-1.nerd");
    parse("test/case/parse-2.nerd");
  }
}
