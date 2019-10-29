// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class PunctuationToken : public Token {
public:
  PunctuationToken(const icu::UnicodeString &value);
  virtual ~PunctuationToken() = default;
  virtual icu::UnicodeString literal() const;
  virtual std::string toString() const;

private:
  icu::UnicodeString value_;
};

} // namespace fastype
