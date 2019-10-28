// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Stringify.h"
#include <string>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class Token : public Stringify {
public:
  // token type
  enum TokenType {
    TT_EOF = 1,        // end of file
    TT_INTEGER = 2,    // integer number
    TT_FLOATING = 3,   // floating number
    TT_OPERATOR = 4,   // operator: + - * / %
    TT_COMPARATOR = 5, // comparator: == != < <= > >=
    TT_ASSIGNMENT = 6, // assignment: =
    TT_BOOLEAN = 7,    // boolean: True False
    TT_IDENTIFIER = 8, // identifier
  };

  static const std::vector<TokenType> &tokenTypes();
  static int tokenTypeValue(TokenType tt);
  static std::string tokenTypeName(TokenType tt);
  static TokenType tokenTypeFromValue(int value);
  static TokenType tokenTypeFromName(const std::string &name);

  // token constants

  // eofs
  const static Sptr<Token> T_EOF;

  // operators
  const static Sptr<Token> T_ADD; // +
  const static Sptr<Token> T_SUB; // -
  const static Sptr<Token> T_MUL; // *
  const static Sptr<Token> T_DIV; // /
  const static Sptr<Token> T_MOD; // %
  const static Sptr<Token> T_NOT; // !

  // assignment
  const static Sptr<Token> T_ASSIGNMENT; // =

  // comparator
  const static Sptr<Token> T_EQ;  // ==
  const static Sptr<Token> T_NEQ; // !=
  const static Sptr<Token> T_LT;  // <
  const static Sptr<Token> T_LE;  // <=
  const static Sptr<Token> T_GT;  // >
  const static Sptr<Token> T_GE;  // >=

  // booleans
  const static Sptr<Token> T_TRUE;  // True
  const static Sptr<Token> T_FALSE; // False

  static const std::vector<Sptr<Token>> eofs();
  static const std::vector<Sptr<Token>> operators();
  static const std::vector<Sptr<Token>> assignments();
  static const std::vector<Sptr<Token>> comparators();
  static const std::vector<Sptr<Token>> booleans();

  Token(TokenType type);
  virtual ~Token() = default;

  virtual const TokenType &type() const;
  virtual long long id() const;

  // token type
  virtual bool isEof() const;
  virtual bool isOperator() const;
  virtual bool isAssignment() const;
  virtual bool isComparator() const;
  virtual bool isBoolean() const;
  virtual bool isInteger() const;
  virtual bool isFloating() const;

  virtual icu::UnicodeString literal() const;
  virtual long long integer() const;
  virtual double floating() const;
  virtual bool boolean() const;

  virtual std::string toString() const;

protected:
  TokenType type_;
  long long id_;
};

} // namespace fastype
