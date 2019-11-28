// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/IntegerConstant.h"
#include <fmt/format.h>

namespace fastype {

IntegerConstant::IntegerConstant(std::shared_ptr<Token> token)
    : token_(token) {}

long long IntegerConstant::value() const { return token_->integer(); }

std::string IntegerConstant::toString() const {
  return fmt::format("[ @IntegerConstant token_:{} ]", token_->toString());
}

Ast::AstType IntegerConstant::type() const {
  return Ast::AstType::INTEGER_CONSTANT;
}

} // namespace fastype
