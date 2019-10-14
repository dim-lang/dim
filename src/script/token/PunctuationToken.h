// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class PunctuationToken : public Token {
public:
  PunctuationToken(int lineNumber, Cowstr literal);
  virtual ~PunctuationToken() = default;
  virtual Cowstr literal() const;
  virtual std::string toString() const;

private:
  Cowstr literal_;
};

} // namespace fastype
