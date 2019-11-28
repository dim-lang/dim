// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"

namespace fastype {

class FloatingToken : public Token {
public:
  FloatingToken(double value);
  virtual ~FloatingToken() = default;
  virtual double floating() const;
  virtual bool equal(const std::shared_ptr<Token> &t) const;
  virtual std::string toString() const;

private:
  double value_;
};

} // namespace fastype
