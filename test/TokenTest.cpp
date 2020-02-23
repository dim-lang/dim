// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Token.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "TokenBuffer.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void go(const char *module) {
  if (!TokenBuffer::pushImport(module)) {
    CASSERT(false, "tokenPushImport {} fail", module);
    return;
  }
  CINFO("go start: {}", module);
  int t;
  while ((t = yylex()) != 0) {
    if (t == T_IDENTIFIER || t == T_I32_CONSTANT || t == T_I8_CONSTANT ||
        t == T_U8_CONSTANT || t == T_I16_CONSTANT || t == T_U16_CONSTANT ||
        t == T_U32_CONSTANT || t == T_I64_CONSTANT || t == T_U64_CONSTANT ||
        t == T_F32_CONSTANT || t == T_F64_CONSTANT || t == T_STRING) {
      CINFO("token:{}, literal:{}", t, yylval.literal);
    } else {
      CINFO("token:{}", t);
    }
  }
  // we don't need pop file manually here
  // tokenPopImport();
  CINFO("go end: {}", module);
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    go("test.case.DslTest1");
    go("test.case.DslTest2");
  }
}
