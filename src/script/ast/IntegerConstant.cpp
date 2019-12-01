// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/IntegerConstant.h"

namespace fastype {

IntegerConstant::IntegerConstant(std::shared_ptr<Token> token)
    : token_(token) {}

IntegerConstant::~IntegerConstant() {
  F_INFO("Destructor this:{}, {}", (void *)this, toString());
}

long long IntegerConstant::value() const { return token_->integer(); }

std::string IntegerConstant::toString() const {
  std::string _1 = token_ ? token_->toString() : "null";
  return fmt::format("[ @IntegerConstant token_:{} ]", _1);
}

Ast::AstType IntegerConstant::type() const {
  return Ast::AstType::INTEGER_CONSTANT;
}

} // namespace fastype
