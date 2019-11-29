// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Token.h"
#include "Logging.h"
#include "Random.h"
#include "catch2/catch.hpp"
#include "script/token/AssignmentToken.h"
#include "script/token/BooleanToken.h"
#include "script/token/ComparatorToken.h"
#include "script/token/EofToken.h"
#include "script/token/FloatingToken.h"
#include "script/token/IntegerToken.h"
#include "script/token/KeywordToken.h"
#include "script/token/OperatorToken.h"
#include "script/token/PunctuationToken.h"
#include "script/token/StringToken.h"
#include <cstdlib>
#include <cstring>
#include <string>

TEST_CASE("Token", "[Token]") {
  SECTION("TokenType") {
    REQUIRE(fastype::Token::tokenTypes().size() == 11);
    REQUIRE(fastype::Token::tokenTypes()[0] ==
            fastype::Token::TokenType::TT_EOF);
    REQUIRE(fastype::Token::tokenTypes()[1] ==
            fastype::Token::TokenType::TT_INTEGER);
    REQUIRE(fastype::Token::tokenTypes()[2] ==
            fastype::Token::TokenType::TT_FLOATING);
    REQUIRE(fastype::Token::tokenTypes()[3] ==
            fastype::Token::TokenType::TT_OPERATOR);
    REQUIRE(fastype::Token::tokenTypes()[4] ==
            fastype::Token::TokenType::TT_COMPARATOR);
    REQUIRE(fastype::Token::tokenTypes()[5] ==
            fastype::Token::TokenType::TT_ASSIGNMENT);
    REQUIRE(fastype::Token::tokenTypes()[6] ==
            fastype::Token::TokenType::TT_BOOLEAN);
    REQUIRE(fastype::Token::tokenTypes()[7] ==
            fastype::Token::TokenType::TT_IDENTIFIER);
    REQUIRE(fastype::Token::tokenTypes()[8] ==
            fastype::Token::TokenType::TT_PUNCTUATION);
    REQUIRE(fastype::Token::tokenTypes()[9] ==
            fastype::Token::TokenType::TT_KEYWORD);
    REQUIRE(fastype::Token::tokenTypes()[10] ==
            fastype::Token::TokenType::TT_STRING);

    REQUIRE(fastype::Token::tokenTypeValue(fastype::Token::TokenType::TT_EOF) ==
            1);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_INTEGER) == 2);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_FLOATING) == 3);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_OPERATOR) == 4);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_COMPARATOR) == 5);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_ASSIGNMENT) == 6);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_BOOLEAN) == 7);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_IDENTIFIER) == 8);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_PUNCTUATION) == 9);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_KEYWORD) == 10);
    REQUIRE(fastype::Token::tokenTypeValue(
                fastype::Token::TokenType::TT_STRING) == 11);

    REQUIRE(fastype::Token::tokenTypeName(fastype::Token::TokenType::TT_EOF) ==
            "TT_EOF");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_INTEGER) == "TT_INTEGER");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_FLOATING) == "TT_FLOATING");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_OPERATOR) == "TT_OPERATOR");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_COMPARATOR) == "TT_COMPARATOR");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_ASSIGNMENT) == "TT_ASSIGNMENT");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_BOOLEAN) == "TT_BOOLEAN");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_IDENTIFIER) == "TT_IDENTIFIER");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_PUNCTUATION) == "TT_PUNCTUATION");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_KEYWORD) == "TT_KEYWORD");
    REQUIRE(fastype::Token::tokenTypeName(
                fastype::Token::TokenType::TT_STRING) == "TT_STRING");

    for (int i = 0; i < (int)fastype::Token::tokenTypes().size(); i++) {
      REQUIRE(fastype::Token::tokenTypeFromValue(fastype::Token::tokenTypeValue(
                  fastype::Token::tokenTypes()[i])) ==
              fastype::Token::tokenTypes()[i]);
      REQUIRE(fastype::Token::tokenTypeFromName(fastype::Token::tokenTypeName(
                  fastype::Token::tokenTypes()[i])) ==
              fastype::Token::tokenTypes()[i]);
    }
  }

  SECTION("eof") {
    REQUIRE(fastype::Token::T_EOF->type() == fastype::Token::TokenType::TT_EOF);
    REQUIRE(fastype::Token::T_EOF->isEof());
    REQUIRE(!fastype::Token::T_EOF->isOperator());
    REQUIRE(!fastype::Token::T_EOF->isAssignment());
    REQUIRE(!fastype::Token::T_EOF->isComparator());
    REQUIRE(!fastype::Token::T_EOF->isBoolean());
    REQUIRE(!fastype::Token::T_EOF->isInteger());
    REQUIRE(!fastype::Token::T_EOF->isPunctuation());
    REQUIRE(!fastype::Token::T_EOF->isKeyword());
    REQUIRE(!fastype::Token::T_EOF->isString());
    REQUIRE(!fastype::Token::T_EOF->toString().empty());
    F_INFO("Token::T_EOF:{}", fastype::Token::T_EOF->toString());
  }

  SECTION("operators") {
    for (auto t : fastype::Token::operators()) {
      REQUIRE(t->isOperator());
      F_INFO("operator: {}", t->toString());
    }
  }

  SECTION("assignments") {
    for (auto t : fastype::Token::assignments()) {
      REQUIRE(t->isAssignment());
      F_INFO("assignment: {}", t->toString());
    }
  }

  SECTION("comparators") {
    for (auto t : fastype::Token::comparators()) {
      REQUIRE(t->isComparator());
      F_INFO("comparator: {}", t->toString());
    }
  }

  SECTION("booleans") {
    for (auto t : fastype::Token::booleans()) {
      REQUIRE(t->isBoolean());
      F_INFO("boolean: {}", t->toString());
    }
  }

  SECTION("strings") {
    std::string s = fastype::Random::nextAlphaNumeric();
    fastype::StringToken st(icu::UnicodeString::fromUTF8(s));
    REQUIRE(st.isString());
    F_INFO("string: {}", st.toString());
  }

  SECTION("integer") {
    int i = fastype::Random::nextInt();
    fastype::IntegerToken it(i);
    REQUIRE(it.isInteger());
    F_INFO("integer: {}", it.toString());
  }

  SECTION("floating") {
    fastype::FloatingToken ft(10.105);
    REQUIRE(ft.isFloating());
    F_INFO("floating: {}", ft.toString());
  }

  SECTION("punctuation") {
    fastype::PunctuationToken pt(UNICODE_STRING_SIMPLE(","));
    REQUIRE(pt.isPunctuation());
    F_INFO("punctuation: {}", pt.toString());
  }

  SECTION("keyword") {
    for (auto t : fastype::Token::keywords()) {
      REQUIRE(t->isKeyword());
      F_INFO("keyword: {}", t->toString());
    }
  }
}
