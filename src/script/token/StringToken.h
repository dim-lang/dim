// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class StringToken : public Token {
public:
  StringToken(int lineNumber, Cowstr literal);
  virtual ~StringToken() = default;
  virtual Cowstr literal() const;
  virtual std::string toString() const;

private:
  Cowstr literal_;
};

} // namespace fastype
