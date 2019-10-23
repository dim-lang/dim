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
  enum TokenType {
    TT_EOF = 1,        // end of file
    TT_INTEGER = 2,    // integer
    TT_OPERATOR = 3,   // operator: + - * / %
    TT_COMPARATOR = 4, // comparator: == != < <= > >=
    TT_ASSIGNMENT = 5, // assignment: =
    TT_BOOLEAN = 6,    // boolean: True False
  };

  static const std::vector<TokenType> &tokenTypes();
  static int tokenTypeValue(TokenType tt);
  static std::string tokenTypeName(TokenType tt);
  static TokenType tokenTypeFromValue(int value);
  static TokenType tokenTypeFromName(const std::string &name);

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

  virtual icu::UnicodeString literal() const;
  virtual long long integer() const;
  virtual bool boolean() const;

  virtual std::string toString() const;

protected:
  TokenType type_;
  long long id_;
};

} // namespace fastype
