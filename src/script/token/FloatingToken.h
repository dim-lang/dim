// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class FloatingToken : public Token {
public:
  FloatingToken(int lineNumber, float value);
  virtual ~FloatingToken() = default;
  virtual float floating() const;
  virtual std::string toString() const;

private:
  float value_;
};

} // namespace fastype
