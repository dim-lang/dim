// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>
#include <vector>

namespace fastype {

class StatementList : public Ast {
public:
  StatementList(const std::vector<std::shared_ptr<Ast>> &children);
  virtual ~StatementList();
  virtual int size() const;
  virtual std::shared_ptr<Ast> get(int i) const;
  virtual std::string toString() const;
  virtual Ast::AstType type() const;

private:
  std::vector<std::shared_ptr<Ast>> children_;
};

} // namespace fastype
