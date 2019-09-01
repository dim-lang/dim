// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class NumberToken : public Token {
public:
  NumberToken(int line, int64_t value);
  virtual ~NumberToken() = default;

  virtual bool isNumber() const;
  virtual int number() const;
  virtual std::string text() const;
  virtual std::string toString() const;

private:
  int64_t value_;
};

} // namespace fastype
