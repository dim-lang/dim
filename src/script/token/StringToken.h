// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class StringToken : public Token {
public:
  virtual ~Token() = default;

  virtual bool isString() const;
  virtual std::string text() const;
  virtual std::string toString() const;

private:
  StringToken(int line, const std::string &value);

  std::string value_;
};

} // namespace fastype
