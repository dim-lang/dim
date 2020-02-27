// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Token.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "catch2/catch.hpp"
#include <cstdio>

static void go(const char *module) {
  union YYSTYPE ys;
  struct YYLTYPE yl;
  Scanner scanner;
  REQUIRE(scanner.push(module) == 1);
  CINFO("go start: {}", module);
  int t;
  while ((t = yylex(&ys, &yl, scanner.scaninfo)) != 0) {
    if (t == T_IDENTIFIER || t == T_I32_CONSTANT || t == T_I8_CONSTANT ||
        t == T_U8_CONSTANT || t == T_I16_CONSTANT || t == T_U16_CONSTANT ||
        t == T_U32_CONSTANT || t == T_I64_CONSTANT || t == T_U64_CONSTANT ||
        t == T_F32_CONSTANT || t == T_F64_CONSTANT || t == T_STRING) {
      CINFO("token:{}, literal:{}", t, ys.literal);
    } else {
      CINFO("token:{}", t);
    }
  }
  // we don't need pop file manually here
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    go("test.case.DslTest1");
    go("test.case.DslTest2");
  }
}
