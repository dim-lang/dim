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
  CompoundStatement(std::vector<Ast *> statementList);
  virtual ~CompoundStatement() = default;
  virtual int size() const;
  virtual Ast *get(int pos) const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  std::vector<Ast *> statementList_;
};

} // namespace fastype
