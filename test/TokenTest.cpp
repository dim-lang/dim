// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Token.h"
#include "catch2/catch.hpp"
#include <cstdio>

extern FILE *yyin;
extern YYSTYPE yylval;

static void parseToken(FILE *fp) {
  yyin = fp;
  int t;
  while ((t = yylex()) != FT_EOF) {
    printf("token:%d, literal:%s\n", yylval.token, yylval.literal.c_str());
  }
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    FILE *fp = std::fopen("test/DslTest1.fast", "r");
    FILE *fp = std::fopen("");
  }
}
