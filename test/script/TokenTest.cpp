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
    REQUIRE(fastype::Token::tokenTypes()[0] == F_TYPE_EOF);
    REQUIRE(fastype::Token::tokenTypes()[1] == F_TYPE_INTEGER);
    REQUIRE(fastype::Token::tokenTypes()[2] == F_TYPE_FLOATING);
    REQUIRE(fastype::Token::tokenTypes()[3] == F_TYPE_OPERATOR);
    REQUIRE(fastype::Token::tokenTypes()[4] == F_TYPE_COMPARATOR);
    REQUIRE(fastype::Token::tokenTypes()[5] == F_TYPE_ASSIGNMENT);
    REQUIRE(fastype::Token::tokenTypes()[6] == F_TYPE_BOOLEAN);
    REQUIRE(fastype::Token::tokenTypes()[7] == F_TYPE_IDENTIFIER);
    REQUIRE(fastype::Token::tokenTypes()[8] == F_TYPE_PUNCTUATION);
    REQUIRE(fastype::Token::tokenTypes()[9] == F_TYPE_KEYWORD);
    REQUIRE(fastype::Token::tokenTypes()[10] == F_TYPE_STRING);

    REQUIRE(F_TYPE_EOF == 1);
    REQUIRE(F_TYPE_INTEGER == 2);
    REQUIRE(F_TYPE_FLOATING == 3);
    REQUIRE(F_TYPE_OPERATOR == 4);
    REQUIRE(F_TYPE_COMPARATOR == 5);
    REQUIRE(F_TYPE_ASSIGNMENT == 6);
    REQUIRE(F_TYPE_BOOLEAN == 7);
    REQUIRE(F_TYPE_IDENTIFIER == 8);
    REQUIRE(F_TYPE_PUNCTUATION == 9);
    REQUIRE(F_TYPE_KEYWORD == 10);
    REQUIRE(F_TYPE_STRING == 11);

    REQUIRE(fastype::Type::nameUTF8(F_TYPE_EOF) == "EOF");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_INTEGER) == "INTEGER");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_FLOATING) == "FLOATING");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_OPERATOR) == "OPERATOR");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_COMPARATOR) == "COMPARATOR");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_ASSIGNMENT) == "ASSIGNMENT");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_BOOLEAN) == "BOOLEAN");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_IDENTIFIER) == "IDENTIFIER");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_PUNCTUATION) == "PUNCTUATION");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_KEYWORD) == "KEYWORD");
    REQUIRE(fastype::Type::nameUTF8(F_TYPE_STRING) == "STRING");

    for (int i = 0; i < (int)fastype::Token::tokenTypes().size(); i++) {
      REQUIRE(fastype::Type::value(
                  fastype::Type::name(fastype::Token::tokenTypes()[i])) ==
              fastype::Token::tokenTypes()[i]);
    }
  }

  SECTION("eof") {
    REQUIRE(fastype::Token::T_EOF->type() == F_TYPE_EOF);
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
