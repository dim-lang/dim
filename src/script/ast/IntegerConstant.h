// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>

namespace fastype {

class IntegerConstant : public Ast {
public:
  IntegerConstant(std::shared_ptr<Token> token);
  virtual ~IntegerConstant() = default;
  virtual long long value() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  std::shared_ptr<Token> token_;
};

} // namespace fastype
