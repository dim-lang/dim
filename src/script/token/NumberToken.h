// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class NumberToken : public Token {
public:
  virtual ~Token() = default;

  virtual bool isNumber() const;
  virtual int number() const;
  virtual std::string text() const;
  virtual std::string toString() const;

private:
  NumberToken(int line, int64_t value);

  int64_t value_;
};

} // namespace fastype
