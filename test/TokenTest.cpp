// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Token.h"
#include "Ast.h"
#include "Log.h"
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
    if (t == T_IDENTIFIER || t == T_INTEGER || t == T_DOUBLE || t == T_STRING) {
      FINFO("token:{}, literal:{}", t, yylval.literal);
    } else {
      FINFO("token:{}", t);
    }
  }
  // we don't need pop file manually here
  // tokenPopFile();
  FINFO("parseToken {} ending...", module);
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
#ifdef YYDEBUG
    yydebug = 1;
#endif
    parseToken("test.DslTest1");
    parseToken("test.DslTest2");
  }
}
