// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Buffer.h"
#include "Log.h"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void parse(const char *fileName) {
  Scanner scanner;
  scanner.bufferStack.push(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("parser parse:{} ast:{}", scanner.compileUnit->toString());
}

TEST_CASE("parser", "[parser]") {
  SECTION("parser") {
    parse("test/case/Parser1.nd");
    parse("test/case/Parser2.nd");
  }
}
