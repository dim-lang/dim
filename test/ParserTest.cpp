// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void parseSuccess(const char *fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("parse:{} ast:{}", fileName, scanner.compileUnit()->toString());
}

static void parseError(const char *fileName) {
  try {
    Scanner scanner(fileName);
    REQUIRE(scanner.parse() != 0);
  } catch (const std::string &e) {
    LOG_WARN("parse with exception:{} in:{}", e, fileName);
  }
}

TEST_CASE("parser", "[parser]") {
  SECTION("success") {
    parseSuccess("test/case/parse-1.nerd");
    parseSuccess("test/case/parse-2.nerd");
    parseSuccess("test/case/parse-3.nerd");
  }

  SECTION("error") {
    parseError("test/case/parse-integer-literal-error-1.nerd");
    parseError("test/case/parse-float-literal-error-1.nerd");
    parseError("test/case/parse-float-literal-error-2.nerd");
  }
}
