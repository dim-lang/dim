// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Token.h"
#include "catch2/catch.hpp"
#include <cstdio>

extern FILE *yyin;
extern YYSTYPE yylval;

static void parseToken(const char *fileName) {
  FILE *fp = std::fopen(fileName, "r");
  yyin = fp;
  int t;
  while ((t = yylex()) != FT_EOF) {
    printf("token:%d, literal:%s\n", yylval.token, yylval.literal.c_str());
  }
  fclose(fp);
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    parseToken("test/DslTest1.fast");
    parseToken("test/DslTest2.fast");
  }
}
