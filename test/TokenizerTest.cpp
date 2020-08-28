// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Log.h"
#include "Scanner.h"
#include "catch2/catch.hpp"

static void tokenize(const char *fileName) {
  Scanner scanner;
  scanner.bufferStack.push(fileName);
  LOG_INFO("tokenizer start: {}", fileName);
  while (true) {
    yy::parser::symbol_type t(scanner.tokenize());
    if (t.kind() == yy::parser::symbol_kind::S_YYEOF) {
      break;
    }
    if (t.kind() == yy::parser::symbol_kind::S_T_INTEGER_LITERAL ||
        t.kind() == yy::parser::symbol_kind::S_T_STRING_LITERAL ||
        t.kind() == yy::parser::symbol_kind::S_T_FLOAT_LITERAL ||
        t.kind() == yy::parser::symbol_kind::S_T_CHARACTER_LITERAL ||
        t.kind() == yy::parser::symbol_kind::S_T_VAR_ID) {
      LOG_INFO("token name:{} string value:{}", t.name(),
               t.value.as<std::string>());
    } else {
      LOG_INFO("token name:{} int value:{}", t.name(), t.value.as<int>());
    }
  }
  // we don't need pop file manually here
}

TEST_CASE("Token", "[Token]") {
  SECTION("Lexer") {
    tokenize("test/case/Parse1.nd");
    tokenize("test/case/Parse2.nd");
  }
}
