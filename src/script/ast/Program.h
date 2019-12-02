// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>
#include <vector>

namespace fastype {

class Program : public Ast {
public:
  Program(std::shared_ptr<Ast> node);
  virtual ~Program() = default;
  virtual std::shared_ptr<Ast> statementList() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  std::shared_ptr<Ast> statementList_;
};

} // namespace fastype
