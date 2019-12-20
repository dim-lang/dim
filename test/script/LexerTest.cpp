// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
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

TEST_CASE("Lexer", "[Lexer]") {
  SECTION("ScriptTest1.fast") {
    icu::UnicodeString data = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/script/ScriptTest1.fast"));
    fastype::Lexer lex(data);
    std::shared_ptr<fastype::Token> t = lex.read();
    REQUIRE(t->isInteger());
    REQUIRE(t->integer() == 1LL);
    t = lex.read();
    REQUIRE(t->isOperator());
    REQUIRE(t == fastype::Token::T_ADD);
    t = lex.read();
    REQUIRE(t->isInteger());
    REQUIRE(t->integer() == 6LL);
    t = lex.read();
    REQUIRE(t->isOperator());
    REQUIRE(t == fastype::Token::T_DIV);
    t = lex.read();
    REQUIRE(t->isPunctuation());
    REQUIRE(t == fastype::Token::T_LP);
    t = lex.read();
    REQUIRE(t->isOperator());
    REQUIRE(t == fastype::Token::T_SUB);
  }
  SECTION("ScriptTest2.fast") {
    icu::UnicodeString data = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/script/ScriptTest2.fast"));
    fastype::Lexer lex(data);
  }
}
