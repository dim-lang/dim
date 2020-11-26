// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include "infra/Log.h"
#include <cstdio>

static void parseSuccess(const char *fileName) {
  Scanner scanner(fileName);
  REQUIRE(scanner.parse() == 0);
}

static void parseError(const char *fileName) {
  try {
    Scanner scanner(fileName);
    REQUIRE(scanner.parse() != 0);
  } catch (const Cowstr &e) {
    LOG_WARN("parse with exception:{} in:{}", e, fileName);
  }
}

TEST_CASE("parser", "[parser]") {
  SECTION("success") {
    parseSuccess("test/case/parse-1.dim");
    parseSuccess("test/case/parse-2.dim");
    parseSuccess("test/case/parse-3.dim");
    parseSuccess("test/case/parse-4.dim");
  }

  SECTION("error") {
    parseError("test/case/parse-integer-literal-error-1.dim");
    parseError("test/case/parse-float-literal-error-1.dim");
    parseError("test/case/parse-float-literal-error-2.dim");
  }
}
