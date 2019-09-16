// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Token.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include "script/token/BooleanToken.h"
#include "script/token/EofToken.h"
#include "script/token/IdToken.h"
#include "script/token/NumberToken.h"
#include "script/token/StringToken.h"
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

TEST_CASE("Token", "[Token]") {
  SECTION("Token::EOF_") {
    REQUIRE(fastype::Token::EOF_->lineNumber() == -1);
    REQUIRE(!fastype::Token::EOF_->isIdentifier());
    REQUIRE(!fastype::Token::EOF_->isNumber());
    REQUIRE(!fastype::Token::EOF_->isBoolean());
    REQUIRE(!fastype::Token::EOF_->isString());
    REQUIRE(!fastype::Token::EOF_->toString().empty());
    F_INFOF("Token::EOF_:{}", fastype::Token::EOF_->toString());
  }

  SECTION("EofToken") {
    int r = rand();
    std::shared_ptr<fastype::Token> t =
        std::shared_ptr<fastype::Token>(new fastype::EofToken(r));
    REQUIRE(t->lineNumber() == r);
    REQUIRE(!t->isIdentifier());
    REQUIRE(!t->isNumber());
    REQUIRE(!t->isBoolean());
    REQUIRE(!t->isString());
    F_INFOF("EofToken:{}", t->toString());
    REQUIRE(!t->toString().empty());
  }

  SECTION("NumberToken") {
    int a = rand();
    int b = rand();
    std::shared_ptr<fastype::Token> t =
        std::shared_ptr<fastype::Token>(new fastype::NumberToken(a, b));
    REQUIRE(t->lineNumber() == a);
    REQUIRE(!t->isIdentifier());
    REQUIRE(t->isNumber());
    REQUIRE(!t->isBoolean());
    REQUIRE(!t->isString());
    F_INFOF("NumberToken:{}", t->toString());
    REQUIRE(!t->toString().empty());
    REQUIRE(t->number() == b);
  }

  SECTION("StringToken") {
    int a = rand();
    std::string b = "string token";
    std::shared_ptr<fastype::Token> t =
        std::shared_ptr<fastype::Token>(new fastype::StringToken(a, b));
    REQUIRE(t->lineNumber() == a);
    REQUIRE(!t->isIdentifier());
    REQUIRE(!t->isNumber());
    REQUIRE(!t->isBoolean());
    REQUIRE(t->isString());
    F_INFOF("StringToken:{}", t->toString());
    REQUIRE(!t->toString().empty());
    REQUIRE(t->text() == b);
  }

  SECTION("IdToken") {
    int a = rand();
    std::string b = "id token";
    std::shared_ptr<fastype::Token> t =
        std::shared_ptr<fastype::Token>(new fastype::IdToken(a, b));
    REQUIRE(t->lineNumber() == a);
    REQUIRE(t->isIdentifier());
    REQUIRE(!t->isNumber());
    REQUIRE(!t->isBoolean());
    REQUIRE(!t->isString());
    INFO(t->toString());
    F_INFOF("IdToken:{}", t->toString());
    REQUIRE(!t->toString().empty());
    REQUIRE(t->text() == b);
  }

  SECTION("BooleanToken") {
    int a = rand();
    bool b = true;
    std::shared_ptr<fastype::Token> t =
        std::shared_ptr<fastype::Token>(new fastype::BooleanToken(a, b));
    REQUIRE(t->lineNumber() == a);
    REQUIRE(!t->isIdentifier());
    REQUIRE(!t->isNumber());
    REQUIRE(t->isBoolean());
    REQUIRE(!t->isString());
    F_INFOF("BooleanToken:{}", t->toString());
    REQUIRE(!t->toString().empty());
    REQUIRE(t->boolean() == b);
  }
}