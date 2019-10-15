// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Token.h"
#include "Logging.h"
#include "exception/NotFoundException.h"
#include "exception/NotImplmentException.h"
#include "script/token/BooleanToken.h"
#include "script/token/EofToken.h"
#include "script/token/IntegerToken.h"
#include "script/token/OperatorToken.h"
#include <fmt/format.h>
#include <string>

namespace fastype {

static Sptr<Token> Token::T_EOF;

static Sptr<Token> Token::T_ADD(UNICODE_STRING("+"));
static Sptr<Token> Token::T_SUB(UNICODE_STRING("-"));
static Sptr<Token> Token::T_MUL(UNICODE_STRING("*"));
static Sptr<Token> Token::T_DIV(UNICODE_STRING("/"));
static Sptr<Token> Token::T_MOD(UNICODE_STRING("%"));

static Sptr<Token> Token::T_ASSIGNMENT(UNICODE_STRING("="));

static Sptr<Token> Token::T_EQ(UNICODE_STRING("=="));
static Sptr<Token> Token::T_NEQ(UNICODE_STRING("!="));
static Sptr<Token> Token::T_LT(UNICODE_STRING("<"));
static Sptr<Token> Token::T_LE(UNICODE_STRING("<="));
static Sptr<Token> Token::T_GT(UNICODE_STRING(">"));
static Sptr<Token> Token::T_GE(UNICODE_STRING(">="));

static Sptr<Token> Token::T_TRUE(UNICODE_STRING("True"));
static Sptr<Token> Token::T_FALSE(UNICODE_STRING("False"));

const std::unordered_set<Sptr<Token>> Token::eofs() {
  const static std::unordered_set<Sptr<Token>> types = {
      Token::T_EOF,
  };
  return types;
}

const std::unordered_set<Sptr<Token>> Token::operators() {
  const static std::unordered_set<Sptr<Token>> types = {
      Token::T_ADD, Token::T_SUB, Token::T_MUL, Token::T_DIV, Token::T_MOD,
  };
  return types;
}

const std::unordered_set<Sptr<Token>> Token::assignments() {
  const static std::unordered_set<Sptr<Token>> types = {
      Token::T_ASSIGNMENT,
  };
  return types;
}

const std::unordered_set<Sptr<Token>> Token::comparators() {
  const static std::unordered_set<Sptr<Token>> types = {
      Token::T_EQ, Token::T_NEQ, Token::T_LT,
      Token::T_LE, Token::T_GT,  Token::T_GE,
  };
  return types;
}

const std::unordered_set<Sptr<Token>> Token::booleans() {
  const static std::unordered_set<Sptr<Token>> types = {
      Token::T_TRUE,
      Token::T_FALSE,
  };
  return types;
}

static long long TokenId = 0LL;

Token::Token(TokenType type) : type_(type), id_(TokenId++) {
  F_INFO("Constructor:{}", toString());
}

const TokenType &Token::type() const { return type_; }

long long Token::id() const { return id_; }

bool Token::isEof() const { return type_ == TokenType::TT_EOF; }

bool Token::isOperator() const { return type_ == TokenType::TT_OPERATOR; }

bool Token::isAssignment() const { return type_ == TokenType::TT_ASSIGNMENT; }

bool Token::isComparator() const { return type_ == TokenType::TT_COMPARATOR; }

bool Token::isBoolean() const { return type_ == TokenType::T_BOOLEAN; }

bool Token::isInteger() const { return type_ == TokenType::T_INTEGER; }

icu::UnicodeString Token::literal() const {
  F_THROW("literal not implement! {}", toString());
}

long long Token::integer() const {
  F_THROW("integer not implement! {}", toString());
}

bool Token::boolean() const {
  F_THROW("boolean not implement! {}", toString());
}

} // namespace fastype
