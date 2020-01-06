// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "dsl/Token.h"
#include "Logging.h"
#include "Random.h"
#include "catch2/catch.hpp"
#include <cstdlib>
#include <cstring>
#include <string>

TEST_CASE("Token", "[Token]") {
  SECTION("Token") {
    // eof
    REQUIRE(dsl::Token::TEOF->type == FDSL_EOF);
    REQUIRE(dsl::Token::TEOF->literal == UNICODE_STRING_SIMPLE("FDSL_EOF"));

    // operator
    REQUIRE(dsl::Token::TADD->type == FDSL_OPERATOR);
    REQUIRE(dsl::Token::TADD->literal == UNICODE_STRING_SIMPLE("+"));

    // comparator
    REQUIRE(dsl::Token::TEQ->type == FDSL_COMPARATOR);
    REQUIRE(dsl::Token::TEQ->literal == UNICODE_STRING_SIMPLE("=="));

    // boolean
    REQUIRE(dsl::Token::TTRUE->type == FDSL_BOOLEAN);
    REQUIRE(dsl::Token::TTRUE->literal == UNICODE_STRING_SIMPLE("True"));

    // punctuation
    REQUIRE(dsl::Token::TLPAREN->type == FDSL_PUNCTUATION);
    REQUIRE(dsl::Token::TLPAREN->literal == UNICODE_STRING_SIMPLE("("));

    // keyword
    REQUIRE(dsl::Token::TLET->type == FDSL_KEYWORD);
    REQUIRE(dsl::Token::TLET->literal == UNICODE_STRING_SIMPLE("let"));
  }
}
