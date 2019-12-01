// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/IdentifierConstant.h"

namespace fastype {

IdentifierConstant::IdentifierConstant(std::shared_ptr<Token> token)
    : token_(token) {}

IdentifierConstant::~IdentifierConstant() {
  F_INFO("Destructor this:{}, {}", (void *)this, toString());
}

icu::UnicodeString IdentifierConstant::value() const {
  return token_->literal();
}

std::string IdentifierConstant::toString() const {
  std::string _1 = token_ ? token_->toString() : "null";
  return fmt::format("[ @IdentifierConstant token_:{} ]", _1);
}

Ast::AstType IdentifierConstant::type() const {
  return Ast::AstType::INTEGER_CONSTANT;
}

} // namespace fastype
