// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class BooleanToken : public Token {
public:
  BooleanToken(int lineNumber, Cowstr literal, TokenType type);
  virtual ~BooleanToken() = default;

  // token type
  virtual bool isEof() const;
  virtual bool isEol() const;
  virtual bool isKeyword() const;
  virtual bool isOperator() const;
  virtual bool isString() const;
  virtual bool isBoolean() const;
  virtual bool isInteger() const;
  virtual bool isFloat() const;
  virtual bool isPunctuation() const;
  virtual bool isComment() const;
  virtual bool isIdentifier() const;

  virtual const Cowstr &text() const;
  virtual int64_t integer() const;
  virtual float floating() const;
  virtual bool boolean() const;

  virtual std::string toString() const = 0;
};

} // namespace fastype
