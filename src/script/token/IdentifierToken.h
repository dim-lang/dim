// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class IdentifierToken : public Token {
public:
  IdentifierToken(int lineNumber, Cowstr literal);
  virtual ~IdentifierToken() = default;
  virtual Cowstr literal() const;
  virtual std::string toString() const;
  bool operator==();

private:
  Cowstr literal_;
};

} // namespace fastype
