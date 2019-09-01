// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class EofToken : public Token {
public:
  EofToken(int line);
  virtual ~EofToken() = default;

  virtual std::string toString() const;
};

} // namespace fastype
