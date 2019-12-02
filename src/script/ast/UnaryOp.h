// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>

namespace fastype {

class UnaryOp : public Ast {
public:
  UnaryOp(std::shared_ptr<Token> op, std::shared_ptr<Ast> expr);
  virtual ~UnaryOp() = default;
  virtual std::shared_ptr<Token> op() const;
  virtual std::shared_ptr<Ast> expr() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  std::shared_ptr<Token> op_;
  std::shared_ptr<Ast> expr_;
};

} // namespace fastype
