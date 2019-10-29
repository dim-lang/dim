// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Token.h"
#include "Logging.h"
#include "exception/Exception.h"
#include "exception/NotFoundException.h"
#include "exception/NotImplementException.h"
#include "script/token/AssignmentToken.h"
#include "script/token/BooleanToken.h"
#include "script/token/ComparatorToken.h"
#include "script/token/EofToken.h"
#include "script/token/IntegerToken.h"
#include "script/token/OperatorToken.h"
#include <fmt/format.h>
#include <string>

namespace fastype {

const std::vector<Token::TokenType> &Token::tokenTypes() {
  const static std::vector<Token::TokenType> types = {
      Token::TokenType::TT_EOF,         Token::TokenType::TT_INTEGER,
      Token::TokenType::TT_FLOATING,    Token::TokenType::TT_OPERATOR,
      Token::TokenType::TT_COMPARATOR,  Token::TokenType::TT_ASSIGNMENT,
      Token::TokenType::TT_BOOLEAN,     Token::TokenType::TT_IDENTIFIER,
      Token::TokenType::TT_PUNCTUATION,
  };
  return types;
}

int Token::tokenTypeValue(Token::TokenType tt) {
  switch (tt) {
  case Token::TokenType::TT_EOF:
    return 1;
  case Token::TokenType::TT_INTEGER:
    return 2;
  case Token::TokenType::TT_FLOATING:
    return 3;
  case Token::TokenType::TT_OPERATOR:
    return 4;
  case Token::TokenType::TT_COMPARATOR:
    return 5;
  case Token::TokenType::TT_ASSIGNMENT:
    return 6;
  case Token::TokenType::TT_BOOLEAN:
    return 7;
  case Token::TokenType::TT_IDENTIFIER:
    return 8;
  case Token::TokenType::TT_PUNCTUATION:
    return 9;
  }
  F_THROW(NotFoundException, "tokenTypeValue not found! tt: {}", (int)tt);
}

std::string Token::tokenTypeName(Token::TokenType tt) {
  switch (tt) {
  case Token::TokenType::TT_EOF:
    return "TT_EOF";
  case Token::TokenType::TT_INTEGER:
    return "TT_INTEGER";
  case Token::TokenType::TT_FLOATING:
    return "TT_FLOATING";
  case Token::TokenType::TT_OPERATOR:
    return "TT_OPERATOR";
  case Token::TokenType::TT_COMPARATOR:
    return "TT_COMPARATOR";
  case Token::TokenType::TT_ASSIGNMENT:
    return "TT_ASSIGNMENT";
  case Token::TokenType::TT_BOOLEAN:
    return "TT_BOOLEAN";
  case Token::TokenType::TT_IDENTIFIER:
    return "TT_IDENTIFIER";
  case Token::TokenType::TT_PUNCTUATION:
    return "TT_PUNCTUATION";
  }
  F_THROW(NotFoundException, "tokenTypeName not found! tt: {}", (int)tt);
}

Token::TokenType Token::tokenTypeFromValue(int value) {
  switch (value) {
  case 1:
    return Token::TokenType::TT_EOF;
  case 2:
    return Token::TokenType::TT_INTEGER;
  case 3:
    return Token::TokenType::TT_FLOATING;
  case 4:
    return Token::TokenType::TT_OPERATOR;
  case 5:
    return Token::TokenType::TT_COMPARATOR;
  case 6:
    return Token::TokenType::TT_ASSIGNMENT;
  case 7:
    return Token::TokenType::TT_BOOLEAN;
  case 8:
    return Token::TokenType::TT_IDENTIFIER;
  case 9:
    return Token::TokenType::TT_PUNCTUATION;
  }
  F_THROW(NotFoundException, "tokenTypeFromValue not found! value: {}", value);
}

Token::TokenType Token::tokenTypeFromName(const std::string &name) {
  for (Token::TokenType tt : tokenTypes()) {
    if (tokenTypeName(tt) == name) {
      return tt;
    }
  }
  F_THROW(NotFoundException, "tokenTypeFromName not found! name: {}", name);
}

const Sptr<Token> Token::T_EOF(new EofToken());

const Sptr<Token> Token::T_ADD(new OperatorToken(UNICODE_STRING_SIMPLE("+")));
const Sptr<Token> Token::T_SUB(new OperatorToken(UNICODE_STRING_SIMPLE("-")));
const Sptr<Token> Token::T_MUL(new OperatorToken(UNICODE_STRING_SIMPLE("*")));
const Sptr<Token> Token::T_DIV(new OperatorToken(UNICODE_STRING_SIMPLE("/")));
const Sptr<Token> Token::T_MOD(new OperatorToken(UNICODE_STRING_SIMPLE("%")));
const Sptr<Token> Token::T_NOT(new OperatorToken(UNICODE_STRING_SIMPLE("!")));

const Sptr<Token>
    Token::T_ASSIGNMENT(new AssignmentToken(UNICODE_STRING_SIMPLE("=")));

const Sptr<Token> Token::T_EQ(new ComparatorToken(UNICODE_STRING_SIMPLE("==")));
const Sptr<Token>
    Token::T_NEQ(new ComparatorToken(UNICODE_STRING_SIMPLE("!=")));
const Sptr<Token> Token::T_LT(new ComparatorToken(UNICODE_STRING_SIMPLE("<")));
const Sptr<Token> Token::T_LE(new ComparatorToken(UNICODE_STRING_SIMPLE("<=")));
const Sptr<Token> Token::T_GT(new ComparatorToken(UNICODE_STRING_SIMPLE(">")));
const Sptr<Token> Token::T_GE(new ComparatorToken(UNICODE_STRING_SIMPLE(">=")));

const Sptr<Token> Token::T_TRUE(new BooleanToken(true));
const Sptr<Token> Token::T_FALSE(new BooleanToken(false));

const Sptr<Token> Token::T_LP;       // (
const Sptr<Token> Token::T_RP;       // )
const Sptr<Token> Token::T_LBRACKET; // [
const Sptr<Token> Token::T_RBRACKET; // ]
const Sptr<Token> Token::T_LBRACE;   // {
const Sptr<Token> Token::T_RBRACE;   // }
const Sptr<Token> Token::T_COMMA;    // ,
const Sptr<Token> Token::T_SEMI;     // ;
const Sptr<Token> Token::T_QUESTION; // ?
const Sptr<Token> Token::T_COLON;    // :

const std::vector<Sptr<Token>> Token::eofs() {
  const static std::vector<Sptr<Token>> types = {
      Token::T_EOF,
  };
  return types;
}

const std::vector<Sptr<Token>> Token::operators() {
  const static std::vector<Sptr<Token>> types = {
      Token::T_ADD, Token::T_SUB, Token::T_MUL,
      Token::T_DIV, Token::T_MOD, Token::T_NOT,
  };
  return types;
}

const std::vector<Sptr<Token>> Token::assignments() {
  const static std::vector<Sptr<Token>> types = {
      Token::T_ASSIGNMENT,
  };
  return types;
}

const std::vector<Sptr<Token>> Token::comparators() {
  const static std::vector<Sptr<Token>> types = {
      Token::T_EQ, Token::T_NEQ, Token::T_LT,
      Token::T_LE, Token::T_GT,  Token::T_GE,
  };
  return types;
}

const std::vector<Sptr<Token>> Token::booleans() {
  const static std::vector<Sptr<Token>> types = {
      Token::T_TRUE,
      Token::T_FALSE,
  };
  return types;
}

static long long TokenId = 0LL;

Token::Token(Token::TokenType type) : type_(type), id_(TokenId++) {
  F_INFO("Constructor:{}", toString());
}

const Token::TokenType &Token::type() const { return type_; }

long long Token::id() const { return id_; }

bool Token::isEof() const { return type_ == Token::TokenType::TT_EOF; }

bool Token::isOperator() const {
  return type_ == Token::TokenType::TT_OPERATOR;
}

bool Token::isAssignment() const {
  return type_ == Token::TokenType::TT_ASSIGNMENT;
}

bool Token::isComparator() const {
  return type_ == Token::TokenType::TT_COMPARATOR;
}

bool Token::isBoolean() const { return type_ == Token::TokenType::TT_BOOLEAN; }

bool Token::isInteger() const { return type_ == Token::TokenType::TT_INTEGER; }

bool Token::isFloating() const {
  return type_ == Token::TokenType::TT_FLOATING;
}

icu::UnicodeString Token::literal() const {
  F_THROW(NotImplementException, "literal not implement! {}", toString());
}

long long Token::integer() const {
  F_THROW(NotImplementException, "integer not implement! {}", toString());
}

double Token::floating() const {
  F_THROW(NotImplementException, "floating not implement! {}", toString());
}

bool Token::boolean() const {
  F_THROW(NotImplementException, "boolean not implement! {}", toString());
}

std::string Token::toString() const {
  return fmt::format("[ @Token type_:{}, id_:{} ]", tokenTypeName(type_), id_);
}

} // namespace fastype
