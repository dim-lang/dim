// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "Parser.tab.hpp"
#include "Token.yy.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void parseToken(const char *fileName) {
  FILE *fp = std::fopen(fileName, "r");
  yyin = fp;
  int t;
  while ((t = yylex()) != 0) {
    if (t == FT_IDENTIFIER || t == FT_INTEGER || t == FT_DOUBLE ||
        t == FT_STRING) {
      FINFO("token:{}, literal:{}", t, yylval.literal);
    } else {
      FINFO("token:{}", t);
    }
  }
  fclose(fp);
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    parseToken("test/DslTest1.fast");
    parseToken("test/DslTest2.fast");
  }
}
