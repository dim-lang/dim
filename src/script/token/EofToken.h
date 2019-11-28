// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"

namespace fastype {

class EofToken : public Token {
public:
  virtual ~EofToken() = default;
  virtual bool equal(const std::shared_ptr<Token> &t) const;
  virtual std::string toString() const;

private:
  EofToken();
};

} // namespace fastype
