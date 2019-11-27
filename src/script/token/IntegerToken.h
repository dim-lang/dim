// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class IntegerToken : public Token {
public:
  IntegerToken(long long value);
  virtual ~IntegerToken() = default;
  virtual long long integer() const;
  virtual bool equal(const Sptr<Token> &t) const;
  virtual std::string toString() const;

private:
  long long value_;
};

} // namespace fastype
