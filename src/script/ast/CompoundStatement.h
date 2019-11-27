// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/Ast.h"
#include "script/Token.h"
#include <vector>

namespace fastype {

class CompoundStatement : public Ast {
public:
  CompoundStatement(Ast *node);
  virtual ~CompoundStatement() = default;
  virtual Ast *statementList() const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  Ast *statementList_;
};

} // namespace fastype
