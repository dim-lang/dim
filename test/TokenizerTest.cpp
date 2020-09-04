// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Log.h"
#include "Scanner.h"
#include "catch2/catch.hpp"
#include <unordered_map>
#include <vector>

static void tokenize(const char *fileName) {
  Scanner scanner(fileName);
  LOG_INFO("start: {}", fileName);
  std::vector<std::string> tokenList;
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
      LOG_INFO("token:{} tag:{}", t.value.as<std::string>(), t.name());
      tokenList.push_back(t.value.as<std::string>());
    } else {
      LOG_INFO("token:{} tag:{}", t.name(), t.value.as<int>());
      tokenList.push_back(t.name());
    }
  }
  std::stringstream ss;
  for (int i = 0; i < (int)tokenList.size(); i++) {
    ss << tokenList[i] << " ";
  }
  LOG_INFO("end: {}", ss.str());
  REQUIRE(true);
}

TEST_CASE("tokenizer", "[tokenizer]") {
  SECTION("tokenize") {
    tokenize("test/case/parse-1.nerd");
    tokenize("test/case/parse-2.nerd");
    tokenize("test/case/parse-integer-literal-error-1.nerd");
    tokenize("test/case/parse-float-literal-error-1.nerd");
    tokenize("test/case/parse-float-literal-error-2.nerd");
  }
}
