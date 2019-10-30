// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class IntegerToken : public Token {
public:
  IntegerToken(long long value, int size);
  virtual ~IntegerToken() = default;
  virtual int size() const;
  virtual long long integer() const;
  virtual std::string toString() const;

private:
  long long value_;
  int size_;
};

} // namespace fastype
