// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>

namespace fastype {

class Parameter : public Ast {
public:
  Parameter(std::shared_ptr<Token> token);
  virtual ~Parameter() = default;
  virtual icu::UnicodeString value() const;
  virtual std::string toString() const;
  virtual int type() const;

private:
  std::shared_ptr<Token> token_;
};

} // namespace fastype
