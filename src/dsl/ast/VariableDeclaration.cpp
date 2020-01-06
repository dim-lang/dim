// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/VariableDeclaration.h"

namespace fastype {

VariableDeclaration::VariableDeclaration(std::shared_ptr<Ast> var,
                                         std::shared_ptr<Ast> expr)
    : var_(var), expr_(expr) {}

std::shared_ptr<Ast> VariableDeclaration::var() const { return var_; }

std::shared_ptr<Ast> VariableDeclaration::expr() const { return expr_; }

std::string VariableDeclaration::toString() const {
  std::string _1 = var_ ? var_->toString() : "null";
  std::string _2 = expr_ ? expr_->toString() : "null";
  return fmt::format("[ @VariableDeclaration var_:{}, expr_:{} ]", _1, _2);
}

int VariableDeclaration::type() const { return F_TYPE_VARIABLE_DECLARATION; }

} // namespace fastype
