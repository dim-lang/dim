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
#include "script/token/KeywordToken.h"
#include "script/token/OperatorToken.h"
#include "script/token/PunctuationToken.h"
#include <atomic>
#include <fmt/format.h>

namespace fastype {

const std::vector<Type> &Token::tokenTypes() {
  static const std::vector<Type> types = {
      Type::T_EOF,      Type::T_INTEGER,    Type::T_FLOATING,
      Type::T_OPERATOR, Type::T_COMPARATOR, Type::T_ASSIGNMENT,
      Type::T_BOOLEAN,  Type::T_IDENTIFIER, Type::T_PUNCTUATION,
      Type::T_KEYWORD,  Type::T_STRING,
  };
  return types;
}

const std::shared_ptr<Token> Token::T_EOF(new EofToken());

const std::shared_ptr<Token>
    Token::T_ADD(new OperatorToken(UNICODE_STRING_SIMPLE("+")));
const std::shared_ptr<Token>
    Token::T_SUB(new OperatorToken(UNICODE_STRING_SIMPLE("-")));
const std::shared_ptr<Token>
    Token::T_MUL(new OperatorToken(UNICODE_STRING_SIMPLE("*")));
const std::shared_ptr<Token>
    Token::T_DIV(new OperatorToken(UNICODE_STRING_SIMPLE("/")));
const std::shared_ptr<Token>
    Token::T_MOD(new OperatorToken(UNICODE_STRING_SIMPLE("%")));
const std::shared_ptr<Token>
    Token::T_NOT(new OperatorToken(UNICODE_STRING_SIMPLE("!")));
const std::shared_ptr<Token>
    Token::T_INC(new OperatorToken(UNICODE_STRING_SIMPLE("++")));
const std::shared_ptr<Token>
    Token::T_DEC(new OperatorToken(UNICODE_STRING_SIMPLE("--")));
const std::shared_ptr<Token>
    Token::T_ADDASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("+=")));
const std::shared_ptr<Token>
    Token::T_SUBASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("-=")));
const std::shared_ptr<Token>
    Token::T_MULASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("*=")));
const std::shared_ptr<Token>
    Token::T_DIVASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("/=")));
const std::shared_ptr<Token>
    Token::T_MODASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("%=")));

const std::shared_ptr<Token>
    Token::T_ASSIGNMENT(new AssignmentToken(UNICODE_STRING_SIMPLE("=")));

const std::shared_ptr<Token>
    Token::T_EQ(new ComparatorToken(UNICODE_STRING_SIMPLE("==")));
const std::shared_ptr<Token>
    Token::T_NEQ(new ComparatorToken(UNICODE_STRING_SIMPLE("!=")));
const std::shared_ptr<Token>
    Token::T_LT(new ComparatorToken(UNICODE_STRING_SIMPLE("<")));
const std::shared_ptr<Token>
    Token::T_LE(new ComparatorToken(UNICODE_STRING_SIMPLE("<=")));
const std::shared_ptr<Token>
    Token::T_GT(new ComparatorToken(UNICODE_STRING_SIMPLE(">")));
const std::shared_ptr<Token>
    Token::T_GE(new ComparatorToken(UNICODE_STRING_SIMPLE(">=")));

const std::shared_ptr<Token> Token::T_TRUE(new BooleanToken(true));
const std::shared_ptr<Token> Token::T_FALSE(new BooleanToken(false));

const std::shared_ptr<Token>
    Token::T_LP(new PunctuationToken(UNICODE_STRING_SIMPLE("(")));
const std::shared_ptr<Token>
    Token::T_RP(new PunctuationToken(UNICODE_STRING_SIMPLE(")")));
const std::shared_ptr<Token>
    Token::T_LBRACKET(new PunctuationToken(UNICODE_STRING_SIMPLE("[")));
const std::shared_ptr<Token>
    Token::T_RBRACKET(new PunctuationToken(UNICODE_STRING_SIMPLE("]")));
const std::shared_ptr<Token>
    Token::T_LBRACE(new PunctuationToken(UNICODE_STRING_SIMPLE("{")));
const std::shared_ptr<Token>
    Token::T_RBRACE(new PunctuationToken(UNICODE_STRING_SIMPLE("}")));
const std::shared_ptr<Token>
    Token::T_COMMA(new PunctuationToken(UNICODE_STRING_SIMPLE(",")));
const std::shared_ptr<Token>
    Token::T_SEMI(new PunctuationToken(UNICODE_STRING_SIMPLE(";")));
const std::shared_ptr<Token>
    Token::T_QUESTION(new PunctuationToken(UNICODE_STRING_SIMPLE("?")));
const std::shared_ptr<Token>
    Token::T_COLON(new PunctuationToken(UNICODE_STRING_SIMPLE(":")));

const std::shared_ptr<Token>
    Token::T_LET(new KeywordToken(UNICODE_STRING_SIMPLE("let")));
const std::shared_ptr<Token>
    Token::T_NULL(new KeywordToken(UNICODE_STRING_SIMPLE("null")));
const std::shared_ptr<Token>
    Token::T_IF(new KeywordToken(UNICODE_STRING_SIMPLE("if")));
const std::shared_ptr<Token>
    Token::T_ELSEIF(new KeywordToken(UNICODE_STRING_SIMPLE("elseif")));
const std::shared_ptr<Token>
    Token::T_ELSE(new KeywordToken(UNICODE_STRING_SIMPLE("else")));
const std::shared_ptr<Token>
    Token::T_FOR(new KeywordToken(UNICODE_STRING_SIMPLE("for")));
const std::shared_ptr<Token>
    Token::T_WHILE(new KeywordToken(UNICODE_STRING_SIMPLE("while")));
const std::shared_ptr<Token>
    Token::T_BREAK(new KeywordToken(UNICODE_STRING_SIMPLE("break")));
const std::shared_ptr<Token>
    Token::T_CONTINUE(new KeywordToken(UNICODE_STRING_SIMPLE("continue")));
const std::shared_ptr<Token>
    Token::T_FUNC(new KeywordToken(UNICODE_STRING_SIMPLE("func")));
const std::shared_ptr<Token>
    Token::T_CLASS(new KeywordToken(UNICODE_STRING_SIMPLE("class")));
const std::shared_ptr<Token>
    Token::T_TYPE(new KeywordToken(UNICODE_STRING_SIMPLE("type")));
const std::shared_ptr<Token>
    Token::T_ISINSTANCE(new KeywordToken(UNICODE_STRING_SIMPLE("isinstance")));
const std::shared_ptr<Token>
    Token::T_IMPORT(new KeywordToken(UNICODE_STRING_SIMPLE("import")));
const std::shared_ptr<Token>
    Token::T_RETURN(new KeywordToken(UNICODE_STRING_SIMPLE("return")));
const std::shared_ptr<Token>
    Token::T_VOID(new KeywordToken(UNICODE_STRING_SIMPLE("void")));

const std::vector<std::shared_ptr<Token>> &Token::eofs() {
  const static std::vector<std::shared_ptr<Token>> types = {
      Token::T_EOF,
  };
  return types;
}

const std::vector<std::shared_ptr<Token>> &Token::operators() {
  static const std::vector<std::shared_ptr<Token>> types = {
      Token::T_ADD,       Token::T_SUB,       Token::T_MUL,
      Token::T_DIV,       Token::T_MOD,       Token::T_NOT,
      Token::T_INC,       Token::T_DEC,       Token::T_ADDASSIGN,
      Token::T_SUBASSIGN, Token::T_MULASSIGN, Token::T_DIVASSIGN,
      Token::T_MODASSIGN,
  };
  return types;
}

const std::vector<std::shared_ptr<Token>> &Token::assignments() {
  static const std::vector<std::shared_ptr<Token>> types = {
      Token::T_ASSIGNMENT,
  };
  return types;
}

const std::vector<std::shared_ptr<Token>> &Token::comparators() {
  static const std::vector<std::shared_ptr<Token>> types = {
      Token::T_EQ, Token::T_NEQ, Token::T_LT,
      Token::T_LE, Token::T_GT,  Token::T_GE,
  };
  return types;
}

const std::vector<std::shared_ptr<Token>> &Token::booleans() {
  static const std::vector<std::shared_ptr<Token>> types = {
      Token::T_TRUE,
      Token::T_FALSE,
  };
  return types;
}

const std::vector<std::shared_ptr<Token>> &Token::punctuations() {
  static const std::vector<std::shared_ptr<Token>> types = {
      Token::T_LP,       Token::T_RP,     Token::T_LBRACKET, Token::T_RBRACKET,
      Token::T_LBRACE,   Token::T_RBRACE, Token::T_COMMA,    Token::T_SEMI,
      Token::T_QUESTION, Token::T_COLON,
  };
  return types;
}

const std::vector<std::shared_ptr<Token>> &Token::keywords() {
  static const std::vector<std::shared_ptr<Token>> types = {
      Token::T_LET,        Token::T_NULL,   Token::T_IF,     Token::T_ELSEIF,
      Token::T_ELSE,       Token::T_FOR,    Token::T_WHILE,  Token::T_BREAK,
      Token::T_CONTINUE,   Token::T_FUNC,   Token::T_CLASS,  Token::T_TYPE,
      Token::T_ISINSTANCE, Token::T_IMPORT, Token::T_RETURN, Token::T_VOID,
  };
  return types;
}

static std::atomic_int64_t TokenId(0LL);

Token::Token(Type type) : type_(type), id_(TokenId++) {}

Token::~Token() { F_INFO("Destructor {}", toString()); }

const Type &Token::type() const { return type_; }

long long Token::id() const { return id_; }

bool Token::isEof() const { return type_ == Type::T_EOF; }

bool Token::isOperator() const { return type_ == Type::T_OPERATOR; }

bool Token::isAssignment() const { return type_ == Type::T_ASSIGNMENT; }

bool Token::isComparator() const { return type_ == Type::T_COMPARATOR; }

bool Token::isBoolean() const { return type_ == Type::T_BOOLEAN; }

bool Token::isInteger() const { return type_ == Type::T_INTEGER; }

bool Token::isFloating() const { return type_ == Type::T_FLOATING; }

bool Token::isIdentifier() const { return type_ == Type::T_IDENTIFIER; }

bool Token::isPunctuation() const { return type_ == Type::T_PUNCTUATION; }

bool Token::isKeyword() const { return type_ == Type::T_KEYWORD; }

bool Token::isString() const { return type_ == Type::T_STRING; }

icu::UnicodeString Token::literal() const {
  F_THROW(NotImplementException, "literal not implement! {}", toString());
}

int64_t Token::integer() const {
  F_THROW(NotImplementException, "integer not implement! {}", toString());
}

double Token::floating() const {
  F_THROW(NotImplementException, "floating not implement! {}", toString());
}

bool Token::boolean() const {
  F_THROW(NotImplementException, "boolean not implement! {}", toString());
}

bool Token::equal(const std::shared_ptr<Token> &t) const {
  F_THROW(NotImplementException, "operator== not implement! {}", toString());
}

std::string Token::toString() const {
  return fmt::format("[ @Token type_:{}, id_:{} ]", type_.nameUTF8(), id_);
}

} // namespace fastype
