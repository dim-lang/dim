// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>

namespace fastype {

class BooleanConstant : public Ast {
public:
  BooleanConstant(std::shared_ptr<Token> token);
  virtual ~BooleanConstant() = default;
  virtual bool value() const;
  virtual std::string toString() const;
  virtual Type type() const;

private:
  std::shared_ptr<Token> token_;
};

} // namespace fastype
