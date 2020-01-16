// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Token.h"
#include "Logging.h"
#include "Parser.tab.hpp"
#include "catch2/catch.hpp"
#include <cstdio>

static void parseToken(const char *module) {
  if (!tokenImportFile(module)) {
    FCHECK(false, "tokenImportFile {} fail", module);
    return;
  }
  FINFO("parseToken {} starting...", module);
  int t;
  while ((t = yylex()) != 0) {
    if (t == FT_IDENTIFIER || t == FT_INTEGER || t == FT_DOUBLE ||
        t == FT_STRING) {
      FINFO("token:{}, literal:{}", t, yylval.literal);
    } else {
      FINFO("token:{}", t);
    }
  }
  tokenPopFile();
  FINFO("parseToken {} ending...", module);
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    parseToken("test.DslTest1");
    parseToken("test.DslTest2");
  }
}
