// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"

namespace fastype {

class IntegerToken : public Token {
public:
  IntegerToken(int64_t value);
  virtual ~IntegerToken() = default;
  virtual int64_t integer() const;
  virtual bool equal(const std::shared_ptr<Token> &t) const;
  virtual std::string toString() const;

private:
  int64_t value_;
};

} // namespace fastype
