// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "dsl/Lexer.h"
#include "Filer.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unicode/unistr.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>

static void lexerParse(const icu::UnicodeString &fileName) {
  icu::UnicodeString data = Filer::read(fileName);
  dsl::Lexer lex(data);
  while (true) {
    std::shared_ptr<Token> t = lex.read();
    if (t->equal(Token::TEOF)) {
      break;
    }
  }
}

TEST_CASE("Lexer", "[Lexer]") {
  SECTION("Lexer") {
    lexerParse(UNICODE_STRING_SIMPLE("test/dsl/DslTest1.fast"));
    lexerParse(UNICODE_STRING_SIMPLE("test/dsl/DslTest2.fast"));
  }
}
