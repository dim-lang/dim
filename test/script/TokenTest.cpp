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
#include "script/token/IntegerToken.h"
#include "script/token/OperatorToken.h"
#include "script/token/StringToken.h"
#include <cstdlib>
#include <cstring>
#include <string>

TEST_CASE("Token", "[Token]") {
  SECTION("TokenType") {
    REQUIRE(fastype::Token::tokenTypes().size() == 9);
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
                fastype::Token::TokenType::TT_IDENTIFIER) == "TT_IDENTIFIER");

    for (int i = 0; i < fastype::Token::tokenTypes().size(); i++) {
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
    REQUIRE(!fastype::Token::T_EOF->toString().empty());
    F_INFO("Token::T_EOF:{}", fastype::Token::T_EOF->toString());
  }

  SECTION("operators") {
    for (auto t : fastype::Token::operators()) {
      F_INFO("operator: {}", t->toString());
    }
  }

  SECTION("assignments") {
    for (auto t : fastype::Token::assignments()) {
      F_INFO("assignment: {}", t->toString());
    }
  }

  SECTION("comparators") {
    for (auto t : fastype::Token::comparators()) {
      F_INFO("comparator: {}", t->toString());
    }
  }

  SECTION("booleans") {
    for (auto t : fastype::Token::booleans()) {
      F_INFO("boolean: {}", t->toString());
    }
  }

  SECTION("strings") {
    std::string s = fastype::Random::nextAlphaNumeric();
    fastype::StringToken st(icu::UnicodeString::fromUTF8(s));
    F_INFO("string: {}", st.toString());
  }

  SECTION("integer") {
    int i = fastype::Random::nextInt();
    fastype::IntegerToken it(i);
    F_INFO("integer: {}", it.toString());
  }
}
