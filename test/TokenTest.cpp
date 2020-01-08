// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Parser.tab.hpp"
#include "catch2/catch.hpp"
#include <cstdio>

static void parseToken(const char *fileName) {
  FILE *fp = std::fopen(fileName, "r");
  yyin = fp;
  int t;
  while ((t = yylex()) != FT_EOF) {
    printf("token:%d, literal:%s\n", yylval.token, yylval.literal);
  }
  fclose(fp);
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    parseToken("test/DslTest1.fast");
    parseToken("test/DslTest2.fast");
  }
}
