// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Log.h"
#include "Scanner.h"
#include "Token.h"
#include "catch2/catch.hpp"
#include "parser.tab.hh"
#include <sstream>
#include <unordered_map>
#include <vector>

static void tokenize(const char *fileName) {
  Scanner scanner(fileName);
  LOG_INFO("start: {}", fileName);
  std::vector<Cowstr> tokenList;
  while (true) {
    Token t = scanner.tokenize();
    if (t.value == 0) {
      break;
    }
    if (tokenIsLiteral(t.value)) {
      LOG_INFO("token:{} tag:{}", t.yylval.literal, tokenName(t.value));
      tokenList.push_back(t.yylval.literal);
      std::free(t.yylval.literal);
    } else {
      LOG_INFO("token:{} tag:{}", tokenName(t.value), t.value);
      tokenList.push_back(tokenName(t.value));
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
    tokenize("test/case/parse-3.nerd");
    tokenize("test/case/parse-4.nerd");
    tokenize("test/case/parse-integer-literal-error-1.nerd");
    tokenize("test/case/parse-float-literal-error-1.nerd");
    tokenize("test/case/parse-float-literal-error-2.nerd");
  }
}
