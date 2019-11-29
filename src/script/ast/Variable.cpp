// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/Variable.h"

namespace fastype {

Variable::Variable(std::shared_ptr<Token> token) : token_(token) {}

icu::UnicodeString Variable::value() const { return token_->literal(); }

std::string Variable::toString() const {
  return fmt::format("[ @Variable token_:{} ]", token_->toString());
}

Ast::AstType Variable::type() const { return Ast::AstType::VARIABLE; }

} // namespace fastype
