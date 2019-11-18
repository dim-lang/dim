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
    fastype::Sptr<fastype::Token> t = lex.read();
    REQUIRE(t->isIdentifier());
    REQUIRE(t->literal() == UNICODE_STRING_SIMPLE("x"));
    t = lex.read();
    REQUIRE(t == fastype::Token::T_ASSIGNMENT);
    t = lex.read();
    REQUIRE(t->isInteger());
    REQUIRE(t->integer() == 1);
    t = lex.read();
    REQUIRE(t->isIdentifier());
    REQUIRE(t->literal() == UNICODE_STRING_SIMPLE("a"));
    t = lex.read();
    REQUIRE(t == fastype::Token::T_ASSIGNMENT);
    t = lex.read();
    REQUIRE(t->isString());
    REQUIRE(t->literal() == UNICODE_STRING_SIMPLE("\"hello world\""));
    t = lex.read();
    REQUIRE(t->isPunctuation());
    REQUIRE(t == fastype::Token::T_SEMI);
  }
  SECTION("LexerTest2.fast") {
    icu::UnicodeString data =
        readFile(UNICODE_STRING_SIMPLE("test/script/LexerTest2.fast"));
    fastype::Lexer lex(data);
    fastype::Sptr<fastype::Token> t = lex.read();
    REQUIRE(t->isKeyword());
    REQUIRE(t == fastype::Token::T_FOR);
    t = lex.read();
    REQUIRE(t->isPunctuation());
    REQUIRE(t == fastype::Token::T_LP);
    t = lex.read();
    REQUIRE(t->isIdentifier());
    REQUIRE(t->literal() == UNICODE_STRING_SIMPLE("i"));
    t = lex.read();
    REQUIRE(t == fastype::Token::T_ASSIGNMENT);
    t = lex.read();
    REQUIRE(t->isInteger());
    REQUIRE(t->integer() == 0);
    t = lex.read();
    REQUIRE(t == fastype::Token::T_SEMI);
  }
}
