// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Token.h"
#include "catch2/catch.hpp"
#include "script/token/EofToken.h"
#include "script/token/IdToken.h"
#include "script/token/NumberToken.h"
#include "script/token/StringToken.h"
#include <cstring>
#include <memory>

TEST_CASE("Token", "[Token]") {
  SECTION("Token::EOF_") {
    REQUIRE(fastype::Token::EOF_->lineNumber() == -1);
    REQUIRE(!fastype::Token::EOF_->isIdentifier());
    REQUIRE(!fastype::Token::EOF_->isNumber());
    REQUIRE(!fastype::Token::EOF_->isString());
    REQUIRE(!fastype::Token::EOF_->toString().empty());
  }
}
