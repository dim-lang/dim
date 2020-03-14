// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Token.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void go(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  CINFO("go start: {}", fileName);
  std::tuple<int, YYSTYPE, YYLTYPE> t;
  while ((t = scanner.tokenize()), std::get<0>(t) != 0) {
    if (std::get<0>(t) == T_IDENTIFIER || std::get<0>(t) == T_I32_CONSTANT ||
        std::get<0>(t) == T_I8_CONSTANT || std::get<0>(t) == T_U8_CONSTANT ||
        std::get<0>(t) == T_I16_CONSTANT || std::get<0>(t) == T_U16_CONSTANT ||
        std::get<0>(t) == T_U32_CONSTANT || std::get<0>(t) == T_I64_CONSTANT ||
        std::get<0>(t) == T_U64_CONSTANT || std::get<0>(t) == T_F32_CONSTANT ||
        std::get<0>(t) == T_F64_CONSTANT || std::get<0>(t) == T_STRING) {
      CINFO("token:{}, literal:{}", std::get<0>(t), std::get<1>(t).literal);
      std::free(std::get<1>(t).literal);
    } else {
      CINFO("token:{}", std::get<0>(t));
    }
  }
  // we don't need pop file manually here
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    go("test/case/DslTest1.co");
    go("test/case/DslTest2.co");
  }
}
