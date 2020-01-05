// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"

namespace fastype {

class BooleanToken : public Token {
public:
  BooleanToken(bool value);
  virtual ~BooleanToken() = default;
  virtual bool boolean() const;
  virtual bool equal(const std::shared_ptr<Token> &t) const;
  virtual std::string toString() const;

private:
  bool value_;
};

} // namespace fastype
