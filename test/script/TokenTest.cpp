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
    REQUIRE(fastype::Token::tokenTypes()[0] == fastype::Type::TP_EOF);
    REQUIRE(fastype::Token::tokenTypes()[1] == fastype::Type::TP_INTEGER);
    REQUIRE(fastype::Token::tokenTypes()[2] == fastype::Type::TP_FLOATING);
    REQUIRE(fastype::Token::tokenTypes()[3] == fastype::Type::TP_OPERATOR);
    REQUIRE(fastype::Token::tokenTypes()[4] == fastype::Type::TP_COMPARATOR);
    REQUIRE(fastype::Token::tokenTypes()[5] == fastype::Type::TP_ASSIGNMENT);
    REQUIRE(fastype::Token::tokenTypes()[6] == fastype::Type::TP_BOOLEAN);
    REQUIRE(fastype::Token::tokenTypes()[7] == fastype::Type::TP_IDENTIFIER);
    REQUIRE(fastype::Token::tokenTypes()[8] == fastype::Type::TP_PUNCTUATION);
    REQUIRE(fastype::Token::tokenTypes()[9] == fastype::Type::TP_KEYWORD);
    REQUIRE(fastype::Token::tokenTypes()[10] == fastype::Type::TP_STRING);

    REQUIRE(fastype::Type::TP_EOF.value() == 1);
    REQUIRE(fastype::Type::TP_INTEGER.value() == 2);
    REQUIRE(fastype::Type::TP_FLOATING.value() == 3);
    REQUIRE(fastype::Type::TP_OPERATOR.value() == 4);
    REQUIRE(fastype::Type::TP_COMPARATOR.value() == 5);
    REQUIRE(fastype::Type::TP_ASSIGNMENT.value() == 6);
    REQUIRE(fastype::Type::TP_BOOLEAN.value() == 7);
    REQUIRE(fastype::Type::TP_IDENTIFIER.value() == 8);
    REQUIRE(fastype::Type::TP_PUNCTUATION.value() == 9);
    REQUIRE(fastype::Type::TP_KEYWORD.value() == 10);
    REQUIRE(fastype::Type::TP_STRING.value() == 11);

    REQUIRE(fastype::Type::TP_EOF.nameUTF8() == "EOF");
    REQUIRE(fastype::Type::TP_INTEGER.nameUTF8() == "INTEGER");
    REQUIRE(fastype::Type::TP_FLOATING.nameUTF8() == "FLOATING");
    REQUIRE(fastype::Type::TP_OPERATOR.nameUTF8() == "OPERATOR");
    REQUIRE(fastype::Type::TP_COMPARATOR.nameUTF8() == "COMPARATOR");
    REQUIRE(fastype::Type::TP_ASSIGNMENT.nameUTF8() == "ASSIGNMENT");
    REQUIRE(fastype::Type::TP_BOOLEAN.nameUTF8() == "BOOLEAN");
    REQUIRE(fastype::Type::TP_IDENTIFIER.nameUTF8() == "IDENTIFIER");
    REQUIRE(fastype::Type::TP_PUNCTUATION.nameUTF8() == "PUNCTUATION");
    REQUIRE(fastype::Type::TP_KEYWORD.nameUTF8() == "KEYWORD");
    REQUIRE(fastype::Type::TP_STRING.nameUTF8() == "STRING");

    for (int i = 0; i < (int)fastype::Token::tokenTypes().size(); i++) {
      REQUIRE(
          fastype::Type::fromValue(fastype::Token::tokenTypes()[i].value()) ==
          fastype::Token::tokenTypes()[i]);
      REQUIRE(fastype::Type::fromName(fastype::Token::tokenTypes()[i].name()) ==
              fastype::Token::tokenTypes()[i]);
    }
  }

  SECTION("eof") {
    REQUIRE(fastype::Token::T_EOF->type().compare(fastype::Type::TP_EOF) == 0);
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
  }

  SECTION("operators") {
    for (auto t : fastype::Token::operators()) {
      REQUIRE(t->isOperator());
    }
  }

  SECTION("assignments") {
    for (auto t : fastype::Token::assignments()) {
      REQUIRE(t->isAssignment());
    }
  }

  SECTION("comparators") {
    for (auto t : fastype::Token::comparators()) {
      REQUIRE(t->isComparator());
    }
  }

  SECTION("booleans") {
    for (auto t : fastype::Token::booleans()) {
      REQUIRE(t->isBoolean());
    }
  }

  SECTION("strings") {
    std::string s = fastype::Random::nextAlphaNumeric();
    fastype::StringToken st(icu::UnicodeString::fromUTF8(s));
    REQUIRE(st.isString());
  }

  SECTION("integer") {
    int i = fastype::Random::nextInt();
    fastype::IntegerToken it(i);
    REQUIRE(it.isInteger());
  }

  SECTION("floating") {
    fastype::FloatingToken ft(10.105);
    REQUIRE(ft.isFloating());
  }

  SECTION("punctuation") {
    fastype::PunctuationToken pt(UNICODE_STRING_SIMPLE(","));
    REQUIRE(pt.isPunctuation());
  }

  SECTION("keyword") {
    for (auto t : fastype::Token::keywords()) {
      REQUIRE(t->isKeyword());
    }
  }
}
