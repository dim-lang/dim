// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Buffer.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include <cstdio>

static void go(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  LOG_INFO("go start: {}", fileName);
  std::tuple<int, YYSTYPE, YYLTYPE> t;
  while ((t = scanner.tokenize()), std::get<0>(t) != 0) {
    if (std::get<0>(t) == T_IDENTIFIER || std::get<0>(t) == T_INTEGER_LITERAL ||
        std::get<0>(t) == T_FLOAT_LITERAL ||
        std::get<0>(t) == T_STRING_LITERAL) {
      LOG_INFO("token:{}, literal:{}", std::get<0>(t), std::get<1>(t).literal);
      std::free(std::get<1>(t).literal);
    } else {
      LOG_INFO("token:{}", std::get<0>(t));
    }
  }
  // we don't need pop file manually here
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    go("test/case/Parser1.shp");
    go("test/case/Parser2.shp");
  }
}
