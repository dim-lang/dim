// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unicode/unistr.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>

static icu::UnicodeString readFile(const icu::UnicodeString &fileName) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), "r", nullptr, "UTF-8");
  UChar *data = nullptr;
  int length = 4096, t = 0, n = 0;
  REQUIRE(fp);

  do {
    if (data == nullptr || n >= length) {
      length = length * 2;
      data = (UChar *)realloc(data, length);
    }

    t = u_file_read(data + n, length - n, fp);
    n += t;
  } while (t > 0);
  u_fclose(fp);
  fp = nullptr;

  return icu::UnicodeString(data, n);
}

TEST_CASE("Lexer", "[Lexer]") {
  SECTION("LexerTest1.fast") {
    icu::UnicodeString data =
        readFile(UNICODE_STRING_SIMPLE("test/script/LexerTest1.fast"));
    fastype::Lexer lex(data);
    lex.parse();
    REQUIRE(lex.peek(0)->isIdentifier());
    REQUIRE(lex.peek(0)->literal() == UNICODE_STRING_SIMPLE("x"));
    REQUIRE(lex.peek(1) == fastype::Token::T_ASSIGNMENT);
    REQUIRE(lex.peek(2)->isInteger());
    REQUIRE(lex.peek(2)->integer() == 1);
    REQUIRE(lex.peek(3)->isIdentifier());
    REQUIRE(lex.peek(3)->literal() == UNICODE_STRING_SIMPLE("a"));
    REQUIRE(lex.peek(4) == fastype::Token::T_ASSIGNMENT);
    REQUIRE(lex.peek(5)->isString());
    REQUIRE(lex.peek(5)->literal() == UNICODE_STRING_SIMPLE("\"hello world\""));
    REQUIRE(lex.peek(6)->isPunctuation());
    REQUIRE(lex.peek(6) == fastype::Token::T_SEMI);
  }
  SECTION("LexerTest2.fast") {
    icu::UnicodeString data =
        readFile(UNICODE_STRING_SIMPLE("test/script/LexerTest2.fast"));
    fastype::Lexer lex(data);
    lex.parse();
    REQUIRE(lex.peek(0)->isKeyword());
    REQUIRE(lex.peek(0) == fastype::Token::T_FOR);
    REQUIRE(lex.peek(1)->isPunctuation());
    REQUIRE(lex.peek(1) == fastype::Token::T_LP);
    REQUIRE(lex.peek(2)->isIdentifier());
    REQUIRE(lex.peek(2)->literal() == UNICODE_STRING_SIMPLE("i"));
    REQUIRE(lex.peek(3) == fastype::Token::T_ASSIGNMENT);
    REQUIRE(lex.peek(4)->isInteger());
    REQUIRE(lex.peek(4)->integer() == 0);
    REQUIRE(lex.peek(5) == fastype::Token::T_SEMI);
    REQUIRE(lex.peek(11) == fastype::Token::T_INC);
    REQUIRE(lex.peek(15) == fastype::Token::T_ADDASSIGN);
  }
}
