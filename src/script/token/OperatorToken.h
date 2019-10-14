// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class OperatorToken : public Token {
public:
  OperatorToken(int lineNumber, Cowstr literal, TokenType ott);
  virtual ~OperatorToken() = default;
  virtual Cowstr literal() const;
  virtual const TokenType &operatorTokenType() const;
  virtual std::string toString() const;

private:
  Cowstr literal_;
  TokenType ott_;
};

} // namespace fastype
