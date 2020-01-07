// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"

namespace fastype {

class ComparatorToken : public Token {
public:
  ComparatorToken(const icu::UnicodeString &value);
  virtual ~ComparatorToken() = default;
  virtual icu::UnicodeString literal() const;
  virtual bool equal(const std::shared_ptr<Token> &t) const;
  virtual std::string toString() const;

private:
  icu::UnicodeString value_;
};

} // namespace fastype