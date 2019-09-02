// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class BooleanToken : public Token {
public:
  BooleanToken(int line, bool value);
  virtual ~BooleanToken() = default;

  virtual bool isBoolean() const;
  virtual bool boolean() const;
  virtual std::string toString() const;

private:
  bool value_;
};

} // namespace fastype
