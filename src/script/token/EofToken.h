// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class EofToken : public Token {
public:
  virtual ~EofToken() = default;
  virtual int size() const;
  virtual std::string toString() const;

private:
  EofToken();

  friend class Token;
};

} // namespace fastype
