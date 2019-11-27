// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class BooleanToken : public Token {
public:
  BooleanToken(Sptr<Token> token);
  virtual ~BooleanToken() = default;
  virtual bool value() const;
  virtual bool equal(const Sptr<Token> &t) const;
  virtual std::string toString() const;

private:
  Sptr<Token> token_;
};

} // namespace fastype
