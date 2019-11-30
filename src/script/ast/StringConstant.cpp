// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/StringConstant.h"

namespace fastype {

StringConstant::StringConstant(std::shared_ptr<Token> token) : token_(token) {}

icu::UnicodeString StringConstant::value() const { return token_->literal(); }

std::string StringConstant::toString() const {
  std::string _1 = token_ ? token_->toString() : "null";
  return fmt::format("[ @StringConstant token_:{} ]", token_->toString());
}

Ast::AstType StringConstant::type() const {
  return Ast::AstType::STRING_CONSTANT;
}

} // namespace fastype
