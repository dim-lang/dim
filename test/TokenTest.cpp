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
    LOG_CHECK(false, "tokenImportFile {} fail", module);
    return;
  }
  LOG_INFO("parseToken {} starting...", module);
  int t;
  while ((t = yylex()) != 0) {
    if (t == T_IDENTIFIER || t == T_I32_CONSTANT || t == T_I8_CONSTANT ||
        t == T_UI8_CONSTANT || t == T_I16_CONSTANT || t == T_UI16_CONSTANT ||
        t == T_UI32_CONSTANT || t == T_I64_CONSTANT || t == T_UI64_CONSTANT ||
        t == T_F32_CONSTANT || t == T_F64_CONSTANT || t == T_STRING) {
      LOG_INFO("token:{}, literal:{}", t, yylval.literal);
    } else {
      LOG_INFO("token:{}", t);
    }
  }
  // we don't need pop file manually here
  // tokenPopFile();
  LOG_INFO("parseToken {} ending...", module);
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
