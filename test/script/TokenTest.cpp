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
  SECTION("eof") {
    REQUIRE(fastype::Token::T_EOF->type() == fastype::TokenType::TT_EOF);
    REQUIRE(!fastype::Token::T_EOF->isEof());
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
