// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/BooleanConstant.h"

namespace fastype {

BooleanConstant::BooleanConstant(std::shared_ptr<Token> token)
    : token_(token) {}

bool BooleanConstant::value() const { return token_->boolean(); }

std::string BooleanConstant::toString() const {
  return fmt::format("[ @BooleanConstant token_:{} ]", token_->toString());
}

Ast::AstType BooleanConstant::type() const {
  return Ast::AstType::BOOLEAN_CONSTANT;
}

} // namespace fastype
