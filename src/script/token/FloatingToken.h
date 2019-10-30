// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class FloatingToken : public Token {
public:
  FloatingToken(double value, int size);
  virtual ~FloatingToken() = default;
  virtual int size() const;
  virtual double floating() const;
  virtual std::string toString() const;

private:
  double value_;
  int size_;
};

} // namespace fastype
