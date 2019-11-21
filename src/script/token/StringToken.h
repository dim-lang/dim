// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class StringToken : public Token {
public:
  StringToken(const icu::UnicodeString &value);
  virtual ~StringToken() = default;
  virtual icu::UnicodeString literal() const;
  virtual bool equal(const Sptr<Token> t) const;
  virtual std::string toString() const;

private:
  icu::UnicodeString value_;
};

} // namespace fastype
