// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "SmartPointer.h"
#include "Stringify.h"
#include "exception/ScriptException.h"
#include "script/TokenType.h"
#include <string>

namespace fastype {

class Token : public Stringify {
public:
  // eofs
  const static Sptr<Token> T_EOF;

  // operators
  const static Sptr<Token> T_ADD;    // +
  const static Sptr<Token> T_SUB;    // -
  const static Sptr<Token> T_MUL;    // *
  const static Sptr<Token> T_DIV;    // /
  const static Sptr<Token> T_MOD;    // %
  const static Sptr<Token> T_ASSIGN; // =
  const static Sptr<Token> T_EQ;     // ==
  const static Sptr<Token> T_NEQ;    // ==
  const static Sptr<Token> T_LT;     // <
  const static Sptr<Token> T_LE;     // <=
  const static Sptr<Token> T_GT;     // >
  const static Sptr<Token> T_GE;     // >=

  // punctuations
  const static Sptr<Token> T_QUESTION;         // ?
  const static Sptr<Token> T_SEMICOLON;        // ;
  const static Sptr<Token> T_COMMA;            // ,
  const static Sptr<Token> T_COLON;            // :
  const static Sptr<Token> T_LEFT_PARENTHESE;  // (
  const static Sptr<Token> T_RIGHT_PARENTHESE; // )
  const static Sptr<Token> T_LEFT_BRACKET;     // [
  const static Sptr<Token> T_RIGHT_BRACKET;    // ]
  const static Sptr<Token> T_LEFT_BRACE;       // {
  const static Sptr<Token> T_RIGHT_BRACE;      // }

  // keywords
  const static Sptr<Token> T_LET;   // let
  const static Sptr<Token> T_FUNC;  // func
  const static Sptr<Token> T_CLASS; // class
  const static Sptr<Token> T_NULL;  // null

  // booleans
  const static Sptr<Token> T_TRUE;  // True
  const static Sptr<Token> T_FALSE; // False

  static const std::unordered_set<Sptr<Token>> punctuations();
  static const std::unordered_set<Sptr<Token>> keywords();
  static const std::unordered_set<Sptr<Token>> eofs();
  static const std::unordered_set<Sptr<Token>> operators();
  static const std::unordered_set<Sptr<Token>> booleans();

  Token(int lineNumber, TokenType type);
  virtual ~Token() = default;

  virtual const int &lineNumber() const;
  virtual const TokenType &type() const;
  virtual long long id() const;

  // token type
  virtual bool isEof() const;
  virtual bool isEol() const;
  virtual bool isKeyword() const;
  virtual bool isOperator() const;
  virtual bool isString() const;
  virtual bool isBoolean() const;
  virtual bool isInteger() const;
  virtual bool isFloating() const;
  virtual bool isPunctuation() const;
  virtual bool isComment() const;
  virtual bool isIdentifier() const;

  virtual Cowstr literal() const;
  virtual long long integer() const;
  virtual float floating() const;
  virtual bool boolean() const;

  virtual std::string toString() = 0;

protected:
  int lineNumber_;
  TokenType type_;
  long long id_;
};

} // namespace fastype
