// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class StringToken : public Token {
public:
  StringToken(int line, const std::string &value);
  virtual ~StringToken() = default;

  virtual bool isString() const;
  virtual std::string text() const;
  virtual std::string toString() const;

private:
  std::string value_;
};

} // namespace fastype
