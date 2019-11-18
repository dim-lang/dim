// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/FloatingConstant.h"
#include <fmt/format.h>

namespace fastype {

FloatingConstant::FloatingConstant(Sptr<Token> token) : token_(token) {}

double FloatingConstant::value() const { return token_->floating(); }

std::string FloatingConstant::toString() const {
  return fmt::format("[ @FloatingConstant token_:{} ]", token_->toString());
}

Ast::AstType FloatingConstant::type() const {
  return Ast::AstType::INTEGER_CONSTANT;
}

} // namespace fastype
