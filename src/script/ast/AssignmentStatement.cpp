// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/AssignmentStatement.h"
#include <fmt/format.h>

namespace fastype {

AssignmentStatement::AssignmentStatement(Sptr<Ast> var, Sptr<Ast> expr)
    : var_(var), expr_(expr) {}

Sptr<Ast> AssignmentStatement::var() const { return var_; }

Sptr<Ast> AssignmentStatement::expr() const { return expr_; }

std::string AssignmentStatement::toString() const {
  return fmt::format("[ @AssignmentStatement var_:{}, expr_:{} ]",
                     var_->toString(), expr_->toString());
}

Ast::AstType AssignmentStatement::type() const {
  return Ast::AstType::ASSIGNMENT_STATEMENT;
}

} // namespace fastype
