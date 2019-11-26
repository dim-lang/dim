// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/StringConstant.h"
#include <fmt/format.h>

namespace fastype {

StringConstant::StringConstant(Sptr<Token> token) : token_(token) {}

icu::UnicodeString StringConstant::value() const { return token_->literal(); }

std::string StringConstant::toString() const {
  return fmt::format("[ @StringConstant token_:{} ]", token_->toString());
}

Ast::AstType StringConstant::type() const {
  return Ast::AstType::INTEGER_CONSTANT;
}

} // namespace fastype
