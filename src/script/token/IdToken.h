// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Token.h"
#include <string>

namespace fastype {

class IdToken : public Token {
public:
  virtual ~Token() = default;

  virtual bool isIdentifier() const;
  virtual std::string text() const;
  virtual std::string toString() const;

private:
  IdToken(int line, const std::string &id);

  std::string id_;
};

} // namespace fastype
