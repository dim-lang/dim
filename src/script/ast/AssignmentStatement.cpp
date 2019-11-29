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
  return fmt::format("[ @AssignmentStatement var_:{}, expr_:{} ]",
                     var_->toString(), expr_->toString());
}

Ast::AstType AssignmentStatement::type() const {
  return Ast::AstType::ASSIGNMENT_STATEMENT;
}

} // namespace fastype
