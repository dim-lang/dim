// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>

namespace fastype {

class StringConstant : public Ast {
public:
  StringConstant(std::shared_ptr<Token> token);
  virtual ~StringConstant() = default;
  virtual icu::UnicodeString value() const;
  virtual std::string toString() const;
  virtual Type type() const;

private:
  std::shared_ptr<Token> token_;
};

} // namespace fastype
