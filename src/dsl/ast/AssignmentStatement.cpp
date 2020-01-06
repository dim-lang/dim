// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/AssignmentStatement.h"

namespace fastype {

AssignmentStatement::AssignmentStatement(std::shared_ptr<Ast> var,
                                         std::shared_ptr<Ast> expr)
    : var_(var), expr_(expr) {}

std::shared_ptr<Ast> AssignmentStatement::var() const { return var_; }

std::shared_ptr<Ast> AssignmentStatement::expr() const { return expr_; }

std::string AssignmentStatement::toString() const {
  std::string _1 = var_ ? var_->toString() : "null";
  std::string _2 = expr_ ? expr_->toString() : "null";
  return fmt::format("[ @AssignmentStatement var_:{}, expr_:{} ]", _1, _2);
}

int AssignmentStatement::type() const { return F_TYPE_ASSIGNMENT_STATEMENT; }

} // namespace fastype
