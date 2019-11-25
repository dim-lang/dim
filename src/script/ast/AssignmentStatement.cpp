// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/AssignmentStatement.h"
#include <fmt/format.h>

namespace fastype {

AssignmentStatement::AssignmentStatement(Sptr<Token> letToken, Ast *left,
                                         Sptr<Token> assignToken, Ast *right)
    : letToken_(letToken), left_(left), assignToken_(assignToken),
      right_(right) {}

double AssignmentStatement::value() const { return token_->floating(); }

std::string AssignmentStatement::toString() const {
  return fmt::format("[ @AssignmentStatement letToken_:{}, left_:{}, "
                     "assignToken_:{}, right_:{} ]",
                     letToken_->toString(), left_->toString(),
                     assignToken_->toString(), right_->toString());
}

Ast::AstType AssignmentStatement::type() const {
  return Ast::AstType::ASSIGNMENT_STATEMENT;
}

} // namespace fastype
